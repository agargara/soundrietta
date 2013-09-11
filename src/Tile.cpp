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
int Tile::render(SDL_Rect& camera, LTexture& gTileTexture, SDL_Rect gTileClips[], SDL_Renderer* gRenderer) const{
    //If the tile is on screen
    if(isCollidingRect(camera, mBox)){
        //Show the tile
        return gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer, &gTileClips[ mType ]);
    }else{
        return 0;
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


bool Tile::isCollidingSphere(SDL_Rect a, SDL_Rect b) const{
    float ax = a.x + (a.w / 2);
    float ay = a.y + (a.h / 2);
    float bx = b.x + (b.w / 2);
    float by = b.y + (b.h / 2);
    
    float distance = getDistance(ax, ay, bx, by);
    if (distance < ((a.w/2) + (b.w/2))){
        return true;
    }else{
        return false;
    }
}

float Tile::getDistance(float ax, float ay, float bx, float by) const{
    return sqrt(((ax-bx)*(ax-bx)) + ((ay-by)*(ay-by)));
}

bool Tile::isCollidingRect(SDL_Rect a, SDL_Rect b) const{
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
        return isCollidingRect(box, mBox);
    }else{
        return false;
    }
}
//@TODO don't use these yet, they're broken
bool Tile::touchesHorizWall(SDL_Rect box) const{
    if(isSolid()){
        if(((box.x + box.w) > mBox.x) && (box.x < (mBox.x + mBox.w))){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}
bool Tile::touchesVertWall(SDL_Rect box) const{
    if(isSolid()){
        if(((box.y + box.h) > mBox.y) && (box.y < (mBox.y + mBox.h))){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

