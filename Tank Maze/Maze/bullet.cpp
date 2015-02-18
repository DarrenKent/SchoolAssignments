#include <iostream>
#include <math.h>
#include "bullet.h"
#include "glut.h"

// Bullet Constructor //
Bullet::Bullet(double scale)
{
	mExplosion = false;
	mExplosionX = 0;
	mExplosionY = 0;
	mExplosionCount = 0;
	mActive = false;
	mVelocity = 200;
	mXPos = 0;
	mYPos = 0;
	mDegrees = 0;
	mScale = scale;
}

// Bullet DeConstructor //
Bullet::~Bullet(){}

// Gets the next X location of the Bullet //
double Bullet::getNextX(double dTime)
{
	return mXPos + cos(((double)mDegrees*03.1415926)/180.0)*dTime*mVelocity;
}

// Gets the next Y location of the Bullet //
double Bullet::getNextY(double dTime)
{
	return mYPos + sin(((double)mDegrees*03.1415926)/180.0)*dTime*mVelocity;
}

// Sets the Bullet's starting location //
void Bullet::setBulletStartPos(double x, double y)
{
	mXPos = x;
	mYPos = y;
}

// Sets the Bullet's angle of fire //
void Bullet::setBulletDegrees(double deg) { mDegrees = deg; }

// Moves the Bullet forward //
void Bullet::moveBullet(Maze * maze,double dTime)
{
	if(mActive)
	{

		if(!maze->isSafe(getNextX(dTime),getNextY(dTime),(2*mScale)))
		{
			mXPos += cos(((double)mDegrees*03.1415926)/180.0)*dTime*mVelocity;
			mYPos += sin(((double)mDegrees*03.1415926)/180.0)*dTime*mVelocity;
		}
		else
		{
			mActive = false;
			mExplosionX = mXPos;
			mExplosionY = mYPos;
			mExplosion = true;
			maze->destroyWallAt(mXPos,mYPos);
		}
	}
}

// Draw the Bullet on the screen //
void Bullet::drawBullet()
{
	if(mActive)
	{
		glPushMatrix();
		glTranslated(mXPos,mYPos,0);
		glRotated(mDegrees,0,0,1);

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(.5,.5,.5);
		glVertex3d(-1*mScale,0,4*mScale);
		for(int i=0; i<32; i++)
		{	
			double theta1 = (double)i/32.0 * 2.0 * 3.1415926;
			double y1 = (.5*mScale) * sin (theta1);
			double z1 = 4*mScale + (.5*mScale) * cos (theta1);
			glVertex3d(-1*mScale, y1 , z1);
		
		}
		glVertex3d(-1*mScale, (.5*mScale) * sin (0.0) , 4*mScale + (.5*mScale) * cos (0.0));
		glEnd();

		for(int i=0; i<32; i++)
		{	
			glBegin(GL_QUADS);
			glColor3d(.3+i/100.0,.3+i/100.0,.3+i/100.0);
			double theta1 = (double)(i+16)/32.0 * 2.0 * 3.1415926;
			double y1 = (.5*mScale) * sin (theta1);
			double z1 = 4*mScale + (.5*mScale) * cos (theta1);

			double theta2 = (double)(i+17)/32.0 * 2.0 * 3.1415926;
			double y2 = (.5*mScale) * sin(theta2);
			double z2 = 4*mScale + (.5*mScale) * cos (theta2);
		
			glVertex3d(-1*mScale, y1 , z1);
			glVertex3d(1*mScale, y1 , z1);
			glVertex3d(1*mScale, y2 , z2);
			glVertex3d(-1*mScale, y2 , z2);
			glEnd();
		}

		glPushMatrix();
		glTranslated(1*mScale,0,4*mScale);
		glRotated(90,0,1,0);
		glutSolidCone(.5*mScale,1*mScale,32,2);
		glPopMatrix();

		glColor3d(0,0,0);
		glBegin(GL_QUADS);
		glVertex2d(-1*mScale,1*mScale);
		glVertex2d(1*mScale,1*mScale);
		glVertex2d(1*mScale,-1*mScale);
		glVertex2d(-1*mScale,-1*mScale);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex2d(1*mScale,1*mScale);
		glVertex2d(2*mScale,0);
		glVertex2d(1*mScale,-1*mScale);
		glEnd();

		glPopMatrix();
	}
}

// Draw the Explosion on impact //
void Bullet::drawExplosion(double dTime,double cellSize)
{
	mExplosionCount += 40.0*dTime;
	if(mExplosionCount <= 15 && mExplosion)
	{
		glColor3d(.5+mExplosionCount/50.0,.3+mExplosionCount/100.0,0);
		glPushMatrix();
		glTranslated(mXPos,mYPos,5);
		glutSolidSphere(mExplosionCount*4,32,8);
		glPopMatrix();
	}
	else
	{
		mExplosionCount = 0;
		mExplosion = false;
	}
}

// Sets Bullet to Active State //
void Bullet::fireBullet(){ mActive = true; };