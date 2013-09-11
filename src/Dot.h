#ifndef DOT_H
#define DOT_H

#include <SDL.h>
#include <vector>
#include <iostream> // for cerr
#include <string>
#include "Util.h"
#include "LTexture.h"
#include "Tile.h"


//The dot that moves around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		// Default accel
		static constexpr float DOT_ACCEL_X = 0.2;
		static constexpr float DOT_ACCEL_Y = 0.2;
		static constexpr float MAX_SPEED = 8;
		static constexpr float MAX_JUMP_SPEED = 8;	

		//Initializes the variables
		Dot(Util& theUtil);

		void processKeys();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and check collision against tiles
		void move( const std::vector<Tile>& tiles );

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera );

		//Shows the dot on the screen
		int render( SDL_Rect& camera, LTexture& gDotTexture, SDL_Renderer* gRenderer );

		// Tests for touching any wall
		bool touchingAnyWall(const std::vector<Tile>& tiles);
		bool touchingHorizWall(float dotX, const std::vector<Tile>& tiles);
		bool touchingVertWall(float dotY, const std::vector<Tile>& tiles);

		// Returns coordinates of dots x, y
		std::string getCoordinates();

    private:
		//Collision box of the dot
		SDL_Rect mBox;

		//The acceleration of the dot
		float mAccelX, mAccelY;

		//The velocity of the dot
		float mVelX, mVelY;

		// In mid-air or not
		bool airborne;
		bool jumping;
		float jumpVel;

		// Utility thing
		Util myUtil;
};

#endif