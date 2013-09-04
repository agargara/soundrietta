#define DEBUG

#include <iostream> // for cerr
#include "Dot.h"

// Constructor
Dot::Dot(){
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

void Dot::handleEvent(SDL_Event& e){
    //If a key was pressed
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch(e.key.keysym.sym)
        {
            case SDLK_UP: if(!airborne){airborne = true; mVelY = -1;} break;
            case SDLK_DOWN: mAccelY += DOT_ACCEL_Y; break;
            case SDLK_LEFT: mAccelX -= DOT_ACCEL_X; break;
            case SDLK_RIGHT: mAccelX += DOT_ACCEL_X; break;
        }
    }else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        //Adjust the velocity
        switch(e.key.keysym.sym){
            case SDLK_UP: mAccelY = 0; break;
            case SDLK_DOWN: mAccelY = 0; break;
            case SDLK_LEFT: mAccelX = 0; break;
            case SDLK_RIGHT: mAccelX = 0; break;
        }
    }
}

void Dot::move(const std::vector<Tile>& tiles){
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
    if((mBox.x < 0) || (mBox.x + DOT_WIDTH > LEVEL_WIDTH) || touchingAnyWall(tiles)){
        //move back
        mBox.x -= mVelX;
    } 
    //Move the dot up or down
    mBox.y += mVelY;
    //If the dot went too far up or down or touched a wall
    if((mBox.y < 0) || (mBox.y + DOT_HEIGHT > LEVEL_HEIGHT) || touchingAnyWall(tiles)){
        //move back
        mBox.y -= mVelY;
        if (mVelY > 0){ // If we were going down, we are no longer airborne
        	airborne = false;
        }
    }else{
		airborne = true;
	}
}

void Dot::setCamera(SDL_Rect& camera){
	//Center the camera over the dot
	camera.x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;
	//Keep the camera in bounds
	if(camera.x < 0){ 
		camera.x = 0;
	}
	if(camera.y < 0){
		camera.y = 0;
	}
	if(camera.x > LEVEL_WIDTH - camera.w){
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if(camera.y > LEVEL_HEIGHT - camera.h){
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Dot::render(SDL_Rect& camera, LTexture gDotTexture, SDL_Renderer* gRenderer){
    //Show the dot
	gDotTexture.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer );
}

bool Dot::touchingAnyWall(const std::vector<Tile>& tiles){
    bool touchingWall = false;

    unsigned int i = 0;
    while(!touchingWall && (i < tiles.size())) {
        if (tiles[i].touchesWall(mBox)){
            touchingWall=true;
        }
        i++;
    }
    return touchingWall;
}

std::string Dot::getCoordinates(){
    Util myUtil = Util();
    return (myUtil.stringify(mBox.x) + ", " + myUtil.stringify(mBox.y));
}