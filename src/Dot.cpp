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
            case SDLK_UP: if(!airborne){airborne = true; mAccelY = -1.5; mVelY = -4;} break;
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
    mBox.x += mVelX;
    mBox.y += mVelY;
    // boundary testing
    if((mBox.x < 0) || (mBox.x + DOT_WIDTH > LEVEL_WIDTH)){ 
        mBox.x -= mVelX;
        mAccelX *= -1; // Reverse direction
    } 
    if((mBox.y < 0) || (mBox.y + DOT_HEIGHT > LEVEL_HEIGHT)){
        mBox.y -= mVelY;
        mAccelY *= -1; // Reverse direction
    }
    std::vector<int> edge = touchingAnyWall(tiles);
    if((edge[0] + edge[1]) > 0){
        mAccelX *= -1; 
        mAccelY *= -1; 
        if (mVelY > 0){
            airborne = false;
        }else{
            airborne = true;
        }
        // @TODO moveToEdge(edge);
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

int Dot::render(SDL_Rect& camera, LTexture& gDotTexture, SDL_Renderer* gRenderer){
    //Show the dot
	return gDotTexture.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer);
}

std::vector<int> Dot::touchingAnyWall(const std::vector<Tile>& tiles){
    std::vector<int> edge = {-1, -1};
    unsigned int i = 0;
    bool touchingWall = false;
    while(!touchingWall && (i < tiles.size())) {
        if (tiles[i].touchesWall(mBox)){
            edge[0] = 1;
            edge[1] = 1;
            touchingWall = true;
        }
        ++i;
    }
    return edge;
}

std::string Dot::getCoordinates(){
    Util myUtil = Util();
    return (myUtil.stringify(mBox.x) + ", " + myUtil.stringify(mBox.y));
}