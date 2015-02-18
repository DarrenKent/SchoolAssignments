#include <iostream>
#include <math.h>
#include "player.h"
#include "glut.h"

// Tank Constructor //
Player::Player(int xSize, int ySize, int cellSize, int borderSize,double scale)
{
	mCollisionShow = true;
	mFired = false;
	mFireCount = 0;
	mXSize = xSize;
	mYSize = ySize;
	mMoveVelocity = 75;
	mTurretVelocity = 75;
	mDegrees = 0;
	mTurretDegrees = 0;
	mCellSize = cellSize;
	mBorderSize = borderSize;
	mXPos = mBorderSize+mCellSize/2;
	mYPos = mBorderSize+mCellSize/2;
	mTankScale = scale;	//Scale of the Tank
}

// Tank Deconstructor //
Player::~Player(){}

// Detrimine if it is safe and move the Player either Forward or Backwards //
void Player::movePlayer(dir direction,double dTime,Maze * maze)
{
	if(direction == D_FORWARD)
	{
		if(!maze->isSafe(getNextX(D_FORWARD,dTime),getNextY(D_FORWARD,dTime),(13.375*mTankScale)))
		{
			mXPos += cos(((double)mDegrees*3.1415926)/180.0)*dTime*mMoveVelocity;
			mYPos += sin(((double)mDegrees*3.1415926)/180.0)*dTime*mMoveVelocity;
		}
		else if(!maze->isSafe(getXPos(),getNextY(D_FORWARD,dTime),(13.375*mTankScale)))
		{
			mYPos += sin(((double)mDegrees*03.1415926)/180.0)*dTime*mMoveVelocity;
		}
		else if(!maze->isSafe(getNextX(D_FORWARD,dTime),getYPos(),(13.375*mTankScale)))
		{
			mXPos += cos(((double)mDegrees*03.1415926)/180.0)*dTime*mMoveVelocity;
		}
	}
	if(direction == D_BACKWARD)
	{
		if(!maze->isSafe(getNextX(D_BACKWARD,dTime),getNextY(D_BACKWARD,dTime),(13.375*mTankScale)))
		{
			mXPos -= cos(((double)mDegrees*03.1415926)/180.0)*dTime*mMoveVelocity;
			mYPos -= sin(((double)mDegrees*03.1415926)/180.0)*dTime*mMoveVelocity;
		}
		else if(!maze->isSafe(getXPos(),getNextY(D_BACKWARD,dTime),(13.375*mTankScale)))
		{
			mYPos -= sin(((double)mDegrees*03.1415926)/180.0)*dTime*mMoveVelocity;
		}
		else if(!maze->isSafe(getNextX(D_BACKWARD,dTime),getYPos(),(13.375*mTankScale)))
		{
			mXPos -= cos(((double)mDegrees*03.1415926)/180.0)*dTime*mMoveVelocity;
		}
	}	
	if(mXPos >(mCellSize*mXSize)+mBorderSize && mYPos > (mCellSize*(mYSize-1))+mBorderSize)
	{
		mXPos = mBorderSize+mCellSize/2;
		mYPos = mBorderSize+mCellSize/2; 
		mDegrees = 0;
		maze->resetMaze();
	}
}

// Get the tanks current X Position //
double Player::getXPos(){ return this->mXPos; }

// Get the tanks current Y Position //
double Player::getYPos(){ return this->mYPos; }

// Rotate the tank //
void Player::rotatePlayer(dir direction,double dTime)
{
	if(direction == D_LEFT)
	{
		mDegrees += mMoveVelocity*dTime;
		mTurretDegrees += mMoveVelocity*dTime;
	}
	if(direction == D_RIGHT)
	{
		mDegrees -= mMoveVelocity*dTime;
		mTurretDegrees -= mMoveVelocity*dTime;
	}
}

// Get the Next X Position of the tank //
double Player::getNextX(dir direction,double dTime)
{
	double newXPos = 0;
	if(direction == D_FORWARD)
		newXPos = mXPos + cos(((double)mDegrees*3.1415926)/180.0)*dTime*mMoveVelocity;
	if(direction == D_BACKWARD)
		newXPos = mXPos - cos(((double)mDegrees*3.1415926)/180.0)*dTime*mMoveVelocity;
	return newXPos;
}

// Get the Next Y Position of the tank //
double Player::getNextY(dir direction,double dTime)
{
	double newYPos = 0;
	if(direction == D_FORWARD)
		newYPos = mYPos + sin(((double)mDegrees*3.1415926)/180.0)*dTime*mMoveVelocity;
	if(direction == D_BACKWARD)
		newYPos = mYPos - sin(((double)mDegrees*3.1415926)/180.0)*dTime*mMoveVelocity;	
	return newYPos;
}

// Rotate the tanks turret //
void Player::rotateTurret(dir direction,double dTime)
{
	if(direction == D_LEFT)
		mTurretDegrees += mTurretVelocity*dTime;
	if(direction == D_RIGHT)
		mTurretDegrees -= mTurretVelocity*dTime;
}

// Set the Fired to True //
void Player::fireCannon() { mFired = true;}

// Get the X Position of the tip of the cannon //
double Player::getCannonX()
{
	double x = mXPos+(24*mTankScale)*cos(mTurretDegrees*3.1415926/180.0);
	return x;
}

// Get the Y Position of the tip of the cannon //
double Player::getCannonY()
{
	double y = mYPos+(24*mTankScale)*sin(mTurretDegrees*3.1415926/180.0);
	return y;
}

// Get the angle of the cannon //
double Player::getCannonDegrees() { return mTurretDegrees; }

// Get the X Position for 3rd Person View //
double Player::getCannon3PersonViewX()
{
	double x = mXPos+(-30*mTankScale)*cos(mTurretDegrees*3.1415926/180.0);
	return x;
}

// Get the Y Position for 3rd Person View //
double Player::getCannon3PersonViewY()
{
	double y = mYPos+(-30*mTankScale)*sin(mTurretDegrees*3.1415926/180.0);
	return y;
}

// Show the Collision circle//
void Player::showCollisionBounds()
{
	glColor3d(1,0,0);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<64; i++)
	{	
		double theta = (double)i/64.0 * 2.0 * 3.1415926;
		double x = (12.375*mTankScale) * cos(theta);
		double y = (12.375*mTankScale) * sin(theta);
		glVertex3d(x, y, 0);
	}
	glEnd();
}

// Draw the Tank to the Scene //
void Player::drawPlayer(SceneManager* render,double dTime, int drawMode)
{


	// Tank //
	glPushMatrix();
	glTranslated(mXPos,mYPos,0);
	glRotated(mDegrees,0,0,1);

	// Collision Circle
	if(mCollisionShow)
		showCollisionBounds();

	render->drawModel("tankBase.obj",drawMode);
	glPopMatrix();

	// Turret //
	glPushMatrix();
	glTranslated(mXPos,mYPos,0);
	glRotated(mTurretDegrees,0,0,1);
	render->drawModel("tankCannon.obj",drawMode);
	glPopMatrix();
}

void Player::setCollisionDetection(bool detection){ mCollisionShow = detection; }
bool Player::getCollisionDetection(){ return mCollisionShow; }