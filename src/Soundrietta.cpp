//#define DEBUG
#include "Soundrietta.h"
using namespace std; 	// For cerr

bool init(){
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		cerr << ("SDL could not initialize! SDL Error: ") << SDL_GetError() << "\n";
		success = false;
	}else{
		//Enable VSync
		if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")){
			cerr << ("Warning: VSync not enabled!");
		}
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			cerr << ("Warning: Linear texture filtering not enabled!");
		}
		//Create window
		gWindow = SDL_CreateWindow("Soundrietta", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		
		if(gWindow == NULL){
			cerr << ("Window could not be created! SDL Error: ") << SDL_GetError() << "\n";
			success = false;
		}else{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL){
				cerr << ("Renderer could not be created! SDL Error: ") << SDL_GetError() << "\n";
				success = false;
			}else{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)){
					cerr << string("SDL_image could not initialize! SDL_image Error: ") << IMG_GetError() << "\n";;
					success = false;
				}
				//Initialize SDL_mixer
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
					cerr << ("SDL_mixer could not initialize! SDL_mixer Error: ") << Mix_GetError() << "\n";
					success = false;
				}
				//Initialize SDL_ttf
				if(TTF_Init() == -1){
					cerr << ("SDL_ttf could not initialize! SDL_ttf Error: ") << TTF_GetError() << "\n";
					success = false;
				}
				//Open the font
				gFont = TTF_OpenFont("res/tarzeau.ttf", 10);
				if(gFont == NULL){
					cerr << ("Failed to load lazy font! SDL_ttf Error: ") << TTF_GetError() << "\n";
					success = false;
				}
			}
		}
	}
	return success;
}
bool loadMedia(vector<Tile>& tiles){
	//Loading success flag
	bool success = true;
	//Load music
	gMusic = Mix_LoadMUS("res/ambient-mystery.wav");
	if(gMusic == NULL){
		cerr << string("Failed to load beat music! SDL_mixer Error: ") << Mix_GetError() << "\n";
		success = false;
	}else{
		#ifdef DEBUG
			cerr << "Loaded music\n";
		#endif
	}
	//Load dot texture
	if(!gDotTexture.loadFromFile("res/dot.bmp", gRenderer)){
		cerr << ("Failed to load dot texture!\n");
		success = false;
	}else{
		#ifdef DEBUG
			cerr << "Loaded dot texture\n";
		#endif
	}
	//Load tile texture
	if(!gTileTexture.loadFromFile("res/tiles.png", gRenderer)){
		cerr << ("Failed to load tile set texture!\n");
		success = false;
	}else{
		#ifdef DEBUG
			cerr << "Loaded tile texture\n";
		#endif
	}
	//Load tile set
	if(!setTiles(tiles)){
		cerr << ("Failed to load tile set!\n");
		success = false;
	}else{
		#ifdef DEBUG
			cerr << "Loaded tile set\n";
		#endif
	}
	#ifdef DEBUG
		cerr << "Tiles size in loadMedia(): "+to_string(tiles.size())+"\n";
	#endif
	return success;
}

void close(){
	//Free loaded images
	gDotTexture.free();
	gTileTexture.free();
	gTextTexture.free();
	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;
	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool setTiles(vector<Tile>& tiles){
	#ifdef DEBUG
		cerr << "Setting tiles\n";
	#endif
	//Success flag
	bool tilesLoaded = true;
    //The tile offsets
    int x = 0, y = 0;
    //Open the map
    ifstream map("res/map01.txt");
    //If the map couldn't be loaded
    if(map == NULL){
		cerr << ("Unable to load map file!\n");
		tilesLoaded = false;
    }else{
		#ifdef DEBUG
			cerr << "Opened map\n";
		#endif
		//Initialize the tiles
		for(int i = 0; i < TOTAL_TILES; ++i){
			int tileType = -1;
			// char* tileChar = NULL;
			//map.getline(tileChar, 4, ','); @TODO file reading is broken
			tileType = 2; // atoi(tileChar);
		    tileType--;
			if(map.fail()){
				//Stop loading map
				cerr << ("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}
			if((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)){
				cerr << "Tiles size before newTile: " << to_string(tiles.size()) << "\n";
				Tile newTile = Tile(x, y, tileType, TILE_SIZE);
				tiles.push_back(newTile);
				cerr << "Pushed back new tile. Tiles size: " << to_string(tiles.size());
			}else{
				//Stop loading map
				cerr << string("Error loading map: Invalid tile type at ") << to_string(i) << "!\n";
				tilesLoaded = false;
				break;
			}
			//Move to next tile spot, wrap at end of row
			x +=  TILE_SIZE;
			if(x >= LEVEL_WIDTH){
				x = 0;
				y +=  TILE_SIZE;
			}
		}	
		//Clip the sprite sheet
		if(tilesLoaded){
			gTileClips[ TILE_RED ].x = 0;
			gTileClips[ TILE_RED ].y = 0;
			gTileClips[ TILE_RED ].w =  TILE_SIZE;
			gTileClips[ TILE_RED ].h =  TILE_SIZE;

			gTileClips[ TILE_GREEN ].x = 0;
			gTileClips[ TILE_GREEN ].y =  TILE_SIZE;
			gTileClips[ TILE_GREEN ].w =  TILE_SIZE;
			gTileClips[ TILE_GREEN ].h =  TILE_SIZE;

			gTileClips[ TILE_BLUE ].x = 0;
			gTileClips[ TILE_BLUE ].y =  TILE_SIZE*2;
			gTileClips[ TILE_BLUE ].w =  TILE_SIZE;
			gTileClips[ TILE_BLUE ].h =  TILE_SIZE;

			gTileClips[ TILE_TOPLEFT ].x =  TILE_SIZE;
			gTileClips[ TILE_TOPLEFT ].y = 0;
			gTileClips[ TILE_TOPLEFT ].w =  TILE_SIZE;
			gTileClips[ TILE_TOPLEFT ].h =  TILE_SIZE;

			gTileClips[ TILE_LEFT ].x =  TILE_SIZE;
			gTileClips[ TILE_LEFT ].y =  TILE_SIZE;
			gTileClips[ TILE_LEFT ].w =  TILE_SIZE;
			gTileClips[ TILE_LEFT ].h =  TILE_SIZE;

			gTileClips[ TILE_BOTTOMLEFT ].x =  TILE_SIZE;
			gTileClips[ TILE_BOTTOMLEFT ].y =  TILE_SIZE*2;
			gTileClips[ TILE_BOTTOMLEFT ].w =  TILE_SIZE;
			gTileClips[ TILE_BOTTOMLEFT ].h =  TILE_SIZE;

			gTileClips[ TILE_TOP ].x =  TILE_SIZE*2;
			gTileClips[ TILE_TOP ].y = 0;
			gTileClips[ TILE_TOP ].w =  TILE_SIZE;
			gTileClips[ TILE_TOP ].h =  TILE_SIZE;

			gTileClips[ TILE_CENTER ].x =  TILE_SIZE*2;
			gTileClips[ TILE_CENTER ].y =  TILE_SIZE;
			gTileClips[ TILE_CENTER ].w =  TILE_SIZE;
			gTileClips[ TILE_CENTER ].h =  TILE_SIZE;

			gTileClips[ TILE_BOTTOM ].x =  TILE_SIZE*2;
			gTileClips[ TILE_BOTTOM ].y =  TILE_SIZE*2;
			gTileClips[ TILE_BOTTOM ].w =  TILE_SIZE;
			gTileClips[ TILE_BOTTOM ].h =  TILE_SIZE;

			gTileClips[ TILE_TOPRIGHT ].x =  TILE_SIZE*3;
			gTileClips[ TILE_TOPRIGHT ].y = 0;
			gTileClips[ TILE_TOPRIGHT ].w =  TILE_SIZE;
			gTileClips[ TILE_TOPRIGHT ].h =  TILE_SIZE;

			gTileClips[ TILE_RIGHT ].x =  TILE_SIZE*3;
			gTileClips[ TILE_RIGHT ].y =  TILE_SIZE;
			gTileClips[ TILE_RIGHT ].w =  TILE_SIZE;
			gTileClips[ TILE_RIGHT ].h =  TILE_SIZE;

			gTileClips[ TILE_BOTTOMRIGHT ].x =  TILE_SIZE*3;
			gTileClips[ TILE_BOTTOMRIGHT ].y =  TILE_SIZE*2;
			gTileClips[ TILE_BOTTOMRIGHT ].w =  TILE_SIZE;
			gTileClips[ TILE_BOTTOMRIGHT ].h =  TILE_SIZE;
		}
	}
    map.close();
	return tilesLoaded;
}

int main(int argc, char* args[]){
	#ifdef DEBUG
		cerr << "Start init\n";
	#endif
	if(!init()){
		cerr << "Failed to initialize!\n";
	}else{
		#ifdef DEBUG
			cerr << "Finish init\n";
		#endif
		// The utility console
		myUtil = Util();
		#ifdef DEBUG
			cerr << "Tiles size: "+to_string(tiles.size())+"\n";
		#endif
		//Load media
		if(!loadMedia(tiles)){
			cerr << "Failed to load media!\n";
		}else{	
			#ifdef DEBUG
				cerr << "Loaded media\n";
				tiles.push_back(Tile(1, 1, 1, 1));
				cerr << "Pushed 1 tile\n";
				cerr << "Tiles size: "+to_string(tiles.size())+"\n";
			#endif
			//Main loop flag
			bool quit = false;
			//Event handler
			SDL_Event e;
			//The dot that will be moving around on the screen
			Dot dot;
			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			// Music
			Mix_PlayMusic(gMusic, -1);
			//While application is running
			while(!quit){
				//Handle events on queue
				while(SDL_PollEvent(&e) != 0){
					//User requests quit
					if(e.type == SDL_QUIT){
						quit = true;
					}
					//Handle input for the dot
					dot.handleEvent(e);
				}
				//Move the dot
				#ifdef DEBUG
					cerr << "Tiles size: "+to_string(tiles.size())+"\n";
				#endif
				dot.move(tiles);
				dot.setCamera(camera);
				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				#ifdef DEBUG
					cerr << "Rendering level...\n";
				#endif
				//Render level
				for(int i = 0; i < TOTAL_TILES; ++i){
					#ifdef DEBUG
						cerr << "Rendering tile "+to_string(i)+"\n";
						cerr << "Tiles size: "+to_string(tiles.size())+"\n";
					#endif
					
					tiles[ i ].render(camera, gTileTexture, gTileClips, gRenderer);
				}
				#ifdef DEBUG
					cerr << "Tiles Rendered\n";
				#endif
				//Render dot
				dot.render(camera, gDotTexture, gRenderer);
				//Write text 
				SDL_Color textColor = { 255, 0, 0 };
				if(!gTextTexture.loadFromRenderedText(consoleCoordinates + myUtil.getConsoleMessage(), textColor, gRenderer, gFont)){
					cerr << "Failed to render text texture!\n";
				}else{
					gTextTexture.render(8, SCREEN_HEIGHT - 18, gRenderer);
				} 
				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}