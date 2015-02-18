#ifndef _BULLET_
#define _BULLET_

#include "maze.h"

class Bullet
{
public:
	Bullet(double scale);
	~Bullet();
	
	double getNextX(double dTime);
	double getNextY(double dTime);

	void setBulletStartPos(double x, double y);
	void setBulletDegrees(double deg);

	void moveBullet(Maze * maze, double dTime);

	void drawBullet();
	void drawExplosion(double dTiem,double cellSize);

	void fireBullet();
	
protected:
private:
	double mExplosionCount;
	double mXPos,mYPos,mVelocity,mDegrees,mScale;
	double mExplosionX,mExplosionY;
	bool mActive,mExplosion;

};

#endif "_BULLET_"