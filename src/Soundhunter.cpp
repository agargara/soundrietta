/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and file streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <string>
#include <fstream>
#include <iostream> /* cout */
#include <cmath>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Tile constants
const int TILE_SIZE = 16;
const int TOTAL_TILE_SPRITES = 12;

//The dimensions of the level
const int TILES_ACROSS = 80;
const int TILES_DOWN = 60;
const int LEVEL_WIDTH = TILE_SIZE * TILES_ACROSS;
const int LEVEL_HEIGHT = TILE_SIZE * TILES_DOWN;

const int TOTAL_TILES = TILES_ACROSS * TILES_DOWN;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_TOPLEFT = 1;
const int TILE_TOP = 2;
const int TILE_TOPRIGHT = 3;
const int TILE_GREEN = 4;
const int TILE_LEFT = 5;
const int TILE_CENTER = 6;
const int TILE_RIGHT = 7;
const int TILE_BLUE = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_BOTTOM = 10;
const int TILE_BOTTOMRIGHT = 11;

// Physics
const float GRAVITY = 1.1;
const float FRICTION = 2;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//The tile
class Tile
{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType );

		//Shows the tile
		void render( SDL_Rect& camera );

		//Get the tile type
		int getType();

		// See if tile is solid
		bool isSolid();

		//Get the collision box
		SDL_Rect getBox();

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		// Default accel
		static const float DOT_ACCEL_X = 0.2;
		static const float DOT_ACCEL_Y = 0.2;
		static const float MAX_SPEED = TILE_SIZE/2;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and check collision against tiles
		void move( Tile *tiles[] );

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera );

		//Shows the dot on the screen
		void render( SDL_Rect& camera );

    private:
		//Collision box of the dot
		SDL_Rect mBox;

		//The acceleration of the dot
		float mAccelX, mAccelY;

		//The velocity of the dot
		float mVelX, mVelY;

		// In mid-air or not
		bool airborne;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia( Tile* tiles[] );

//Frees media and shuts down SDL
void close();

//Box collision detectors
bool isColliding( SDL_Rect a, SDL_Rect b );
int collisionSide( SDL_Rect a, SDL_Rect b );

//Checks collision box against set of tiles
bool touchesWall( SDL_Rect box, Tile* tiles[] );

//Sets tiles from tile map
bool setTiles( Tile *tiles[] );

// Gets absolute value of floats
float abs(float n);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gTileTexture;
LTexture gTextTexture;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

//Globally used font
TTF_Font *gFont = NULL;
//The music that will be played
Mix_Music *gMusic = NULL;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

Tile::Tile( int x, int y, int tileType )
{
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w =  TILE_SIZE;
    mBox.h =  TILE_SIZE;

    //Get the tile type
    mType = tileType;
}

void Tile::render( SDL_Rect& camera )
{
    //If the tile is on screen
    if( isColliding( camera, mBox ) )
    {
        //Show the tile
        gTileTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
    }
}

int Tile::getType()
{
    return mType;
}

bool Tile::isSolid()
{
	if (mType == 0 | mType == 4 | mType == 8){
		return false;
	}
	else{
		return true;
	}
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

Dot::Dot()
{
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

    //Initialize the velocity and accel
    mVelX = 0;
    mVelY = 0;
    mAccelX = 0;
    mAccelY = 0;

    airborne = true;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: if(!airborne){airborne = true; mVelY = -1;} break;
            case SDLK_DOWN: mAccelY += DOT_ACCEL_Y; break;
            case SDLK_LEFT: mAccelX -= DOT_ACCEL_X; break;
            case SDLK_RIGHT: mAccelX += DOT_ACCEL_X; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mAccelY = 0; break;
            case SDLK_DOWN: mAccelY = 0; break;
            case SDLK_LEFT: mAccelX = 0; break;
            case SDLK_RIGHT: mAccelX = 0; break;
        }
    }
}

void Dot::move( Tile *tiles[] )
{
	// Apply acceleration
	if(fabs(mVelX) < MAX_SPEED){
		mVelX += mAccelX;
	}
	if(fabs(mVelY) < MAX_SPEED){
		mVelY += mAccelY;
	}
	if(mVelY < MAX_SPEED && airborne){
		mVelY += GRAVITY;
	}

	// Apply deceleration
	if(mAccelX == 0){
		mVelX /= FRICTION;
	}

	if (!airborne){
			mAccelY = -2;
	}
	
	//Move the dot left or right
    mBox.x += mVelX;
    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    if (airborne){
	    mBox.y += mVelY;
	    //If the dot went too far up or down or touched a wall
	    if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
	    {
	        //move back
	        mBox.y -= mVelY;
	        if (mVelY > 0){ // If we were going down, we are no longer airborne
	        	airborne = false;
	        }
	    }
	}
}

void Dot::setCamera( SDL_Rect& camera )
{
	//Center the camera over the dot
	camera.x = ( mBox.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if( camera.x < 0 )
	{ 
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Dot::render( SDL_Rect& camera )
{
    //Show the dot
	gDotTexture.render( mBox.x - camera.x, mBox.y - camera.y );
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Enable VSync
		if( !SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" ) )
		{
			printf( "Warning: VSync not enabled!" );
		}

		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Soundrietta", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				//Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}

				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia( Tile* tiles[] )
{
	//Loading success flag
	bool success = true;

	//Load music
	gMusic = Mix_LoadMUS( "res/ambient-mystery.wav" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	//Load dot texture
	if( !gDotTexture.loadFromFile( "res/dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load tile texture
	if( !gTileTexture.loadFromFile( "res/tiles.png" ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	//Load tile map
	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont( "res/tarzeau.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	gDotTexture.free();
	gTileTexture.free();
	gTextTexture.free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool isColliding( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

/**
*    Checks each side for collison. Returns:
*    0 : no collision
*    1 : collision on top of A
*    2 : collision on right of A
*    3 : collision on bottom of A
*    4 : collision on left of A  
*/
int collisionSide( SDL_Rect a, SDL_Rect b )
{
	float w = 0.5 * (a.w + b.w);
	float h = 0.5 * (a.h + b.h);

	float aCenterX = (a.x + a.x + w)/2;
	float bCenterX = (b.x + b.x + w)/2;
	float aCenterY = (a.y + a.y + w)/2;
	float bCenterY = (b.y + b.y + w)/2;

	float dx = aCenterX - bCenterX;
	float dy = aCenterY - bCenterY;

	if (fabs(dx) <= w && fabs(dy) <= h){
	    // collision!
	    float wy = w * dy;
	    float hx = h * dx;

	    if (wy > hx){
	        if (wy > -hx){
	            return 1;
	        }else{
	            return 4;
	        }
	    }else{
	        if (wy > -hx){
	            return 2;
	        }else{
	            return 3;
	        }
	    }
	}else{
		return 0; // no collision
	}
}

bool setTiles( Tile* tiles[] )
{
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "res/map01.txt" );

    //If the map couldn't be loaded
    if( map == NULL )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < TOTAL_TILES; ++i )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			char* tileChar;
			map.getline(tileChar, 4, ','); // Read 4 characters or until comma
			tileType = atoi(tileChar);
		    tileType--;
			

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
			{
				tiles[ i ] = new Tile( x, y, tileType );
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x +=  TILE_SIZE;

			//If we've gone too far
			if( x >= LEVEL_WIDTH )
			{
				//Move back
				x = 0;

				//Move to the next row
				y +=  TILE_SIZE;
			}
		}
		
		//Clip the sprite sheet
		if( tilesLoaded )
		{
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

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] )
{
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        //If the tile is solid
        if( tiles[ i ]->isSolid() ){
            if(isColliding( box, tiles[ i ]->getBox() )){
            	return false;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

float abs(float n) { 
	if (n < 0) { 
		return (-1 * n); 
	} 
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//The level tiles
		Tile* tileSet[ TOTAL_TILES ];

		//Load media
		if( !loadMedia( tileSet ) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot;

			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			// Music
			Mix_PlayMusic( gMusic, -1 );

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					dot.handleEvent( e );
				}

				//Move the dot
				dot.move( tileSet );
				dot.setCamera( camera );

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render level
				for( int i = 0; i < TOTAL_TILES; ++i )
				{
					tileSet[ i ]->render( camera );
				}

				//Render dot
				dot.render( camera );

				//Render text
				gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}