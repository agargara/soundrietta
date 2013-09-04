#ifndef SOUNDRIETTA_H
#define SOUNDRIETTA_H

#include <fstream>
#include <iostream> /* cout cerr */
#include <cmath>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Util.h"
#include "Tile.h"
#include "Dot.h"
#include "LTexture.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia( std::vector<Tile>& tiles );

//Frees media and shuts down SDL
void close();

//Sets tiles from tile map
bool setTiles( std::vector<Tile>& tiles);

//The window we'll be rendering to
SDL_Window* gWindow;

//The window renderer
SDL_Renderer* gRenderer;

//Scene textures
LTexture gDotTexture;
LTexture gTileTexture;
LTexture gTextTexture;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

//Globally used font
TTF_Font *gFont;

//The music that will be played
Mix_Music *gMusic;

// Coordinates for console display
std::string consoleCoordinates;

// Util for console message
Util myUtil;

// The vector of tiles
std::vector<Tile> tiles;

#endif