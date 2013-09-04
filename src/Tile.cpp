#define DEBUG

#include "Tile.h"

// Constructor
Tile::Tile(int x, int y, int tileType, int tileSize){
    //Get the offsets
    mBox.x = x;
    mBox.y = y;
    //Set the collision box
    mBox.w =  tileSize;
    mBox.h =  tileSize;
    //Get the tile type
    mType = tileType;
}
void Tile::render(SDL_Rect& camera, LTexture gTileTexture, SDL_Rect gTileClips[], SDL_Renderer* gRenderer) const{
    //If the tile is on screen
    if(isColliding(camera, mBox)){
        //Show the tile
        gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer, &gTileClips[ mType ]);
    }
}
int Tile::getType() const{
    return mType;
}
bool Tile::isSolid() const{
	if ((mType == 0) | (mType == 4) | (mType == 8)){
		return false;
	}else{
		return true;
	}
}
SDL_Rect Tile::getBox() const{
    return mBox;
}
bool Tile::isColliding(SDL_Rect a, SDL_Rect b) const{
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
    if(bottomA <= topB){
        return false;
    }
    if(topA >= bottomB){
        return false;
    }
    if(rightA <= leftB){
        return false;
    }
    if(leftA >= rightB){
        return false;
    }
    //If none of the sides from A are outside B
    return true;
}
bool Tile::touchesWall(SDL_Rect box) const{
    if(isSolid()){
        if(isColliding(box, mBox)){
            return true;
        }
    }
    return false;
}