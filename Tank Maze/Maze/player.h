#ifndef _PLAYER_
#define _PLAYER_

#include "maze.h"
#include "sceneManager.h"

class Player
{
public:
	enum dir
	{
		D_FORWARD = 0,
		D_BACKWARD,
		D_LEFT,
		D_RIGHT
	};
	Player(int xSize, int ySize, int cellSize, int borderSize, double scale);
	~Player();

	// Tank Related Methods //
	void movePlayer(dir direction,double dTime,Maze * maze);
	void rotatePlayer(dir direction,double dTime);
	double getXPos();
	double getYPos();
	double getNextX(dir direction,double dTime);
	double getNextY(dir direction,double dTime);

	// Cannon Related Methods //
	void rotateTurret(dir direction,double dTime);
	void fireCannon();
	double getCannonX();
	double getCannonY();
	double getCannonDegrees();
	double getCannon3PersonViewX();
	double getCannon3PersonViewY();

	// Draw Tank //
	void showCollisionBounds();
	void drawPlayer(SceneManager* render,double dTime, int drawMode);
	
	void setCollisionDetection(bool detection);
	bool getCollisionDetection();
protected:
private:
	// Tank Related Variables //
	double mXPos,mYPos,mMoveVelocity,mDegrees,mTankScale;
	bool mCollisionShow;

	// Turret Related Variables
	double mTurretDegrees,mTurretVelocity,mFireCount;
	bool mFired;

	// Maze Information //
	int mXSize,mYSize,mCellSize,mBorderSize;
};

#endif "_PLAYER_"