#define DEBUG

#include "Dot.h"

// Constructor
Dot::Dot(Util& theUtil){
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
    myUtil = theUtil;
}

void Dot::handleEvent(SDL_Event& e){
    //If a key was pressed
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch(e.key.keysym.sym)
        {
            case SDLK_UP:   if(!airborne){mVelY = -JUMP_SPEED;}
                            break;
            case SDLK_DOWN: break;
            case SDLK_LEFT: mAccelX -= DOT_ACCEL_X; break;
            case SDLK_RIGHT: mAccelX += DOT_ACCEL_X; break;
        }
    }else if(e.type == SDL_KEYUP && e.key.repeat == 0){ // Release key
        // Adjust the velocity
        switch(e.key.keysym.sym){
            case SDLK_UP: mAccelY = 0; break;
            case SDLK_DOWN: mAccelY = 0; break;
            case SDLK_LEFT: mAccelX = 0; break;
            case SDLK_RIGHT: mAccelX = 0; break;
        }
    }
}

void Dot::move(const std::vector<Tile>& tiles){
    airborne = true;
	float dotX = mBox.x;
    float dotY = mBox.y;

    if((mVelY + GRAVITY) < MAX_SPEED){
        mVelY += GRAVITY; // Constant gravity
    }
    // Apply acceleration
    if(fabs(mVelX + mAccelX) < MAX_SPEED){
        mVelX += mAccelX;
    }
    if(fabs(mVelY + mAccelY) < MAX_SPEED){
        mVelY += mAccelY;
    }
    // Apply deceleration
    if(mAccelX == 0){
        mVelX /= FRICTION;
    }
    if(fabs(mVelX) < .1){
        mVelX = 0;
    }

    // horiz movement tests
    dotX += mVelX;
    if((dotX < 0) || (dotX + DOT_WIDTH > LEVEL_WIDTH) || touchingHorizWall(dotX, tiles)){ 
        dotX -= mVelX;
        mAccelX = 0;
        mVelX *= -1; // Reverse direction
        mVelX /= FRICTION; // Apply friction
    } 
    // vertical movement tests
    dotY += mVelY;
    if((dotY < 0) || (dotY + DOT_HEIGHT > LEVEL_HEIGHT) || touchingVertWall(dotY, tiles)){
        dotY -= mVelY;
        if(mVelY > 0){ // If we were going down, stop.
            airborne = false;
        }else{
            dotY -= mVelY;
            mVelY *= -1; // Reverse direction
            mVelY /= FRICTION; // Apply friction
        }
    }

    mBox.x = int(dotX + 0.5);
    mBox.y = int(dotY + 0.5);
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

int Dot::render(SDL_Rect& camera, LTexture& gDotTexture, SDL_Renderer* gRenderer){
    //Show the dot
	return gDotTexture.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer);
}

bool Dot::touchingAnyWall(const std::vector<Tile>& tiles){
    bool touchingWall = false;
    unsigned int i = 0;
    while(!touchingWall && (i < tiles.size())) {
        if (tiles[i].touchesWall(mBox)){
            touchingWall = true;
        }
        ++i;
    }
    return touchingWall;
}
bool Dot::touchingHorizWall(float dotX, const std::vector<Tile>& tiles){
    bool touchingWall = false;
    unsigned int i = 0;
    SDL_Rect newBox;
    newBox.x = dotX;
    newBox.y = mBox.y;
    newBox.w = mBox.w;
    newBox.h = mBox.h;
    while(!touchingWall && (i < tiles.size())) {
        if (tiles[i].touchesWall(newBox)){
            touchingWall = true;
        }
        ++i;
    }
    return touchingWall;
}
bool Dot::touchingVertWall(float dotY, const std::vector<Tile>& tiles){
    bool touchingWall = false;
    unsigned int i = 0;
    SDL_Rect newBox;
    newBox.x = mBox.x;
    newBox.y = dotY;
    newBox.w = mBox.w;
    newBox.h = mBox.h;
    while(!touchingWall && (i < tiles.size())) {
        if (tiles[i].touchesWall(newBox)){
            touchingWall = true;
        }
        ++i;
    }
    return touchingWall;
}

std::string Dot::getCoordinates(){
    return (myUtil.stringify(mBox.x) + ", " + myUtil.stringify(mBox.y));
}