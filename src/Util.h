#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <stdexcept>
#include <sstream>

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


class Util{
	public:
		//Initializes the variables
		Util();

		// set console message
		void setConsoleMessage(std::string message);

		// get console message
		std::string getConsoleMessage();

		// Float -> String
		std::string stringify(float x);

	private:
		std::string consoleMessage;
};

#endif