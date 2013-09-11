#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include <vector>
#include "LTexture.h"
#include <iostream> // for cerr

//The tile
class Tile
{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType, int tileSize);

		//Shows the tile
		int render( SDL_Rect& camera,  LTexture& gTileTexture, SDL_Rect gTileClips[], SDL_Renderer* gRenderer) const;

		//Get the tile type
		int getType() const;

		// See if tile is solid
		bool isSolid() const;

		//Get the collision box
		SDL_Rect getBox() const;

		//Box collision detectors
		bool isCollidingSphere( SDL_Rect a, SDL_Rect b ) const;

		// Distance between two points
		float getDistance(float ax, float ay, float bx, float by) const;

		bool isCollidingRect( SDL_Rect a, SDL_Rect b ) const;

		//Checks collision box against set of tiles
		bool touchesWall( SDL_Rect box) const;

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};
#endif