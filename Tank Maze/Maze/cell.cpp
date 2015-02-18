#include "glut.h"
#include "cell.h"

// Cell's Constructor //
Cell::Cell(double x1, double y1, double x2, double y2,int xsize, int ysize)
{
	this->mXSize = xsize;
	this->mYSize = ysize;
	this->mXPos1 = x1;
	this->mYPos1 = y1;
	this->mXPos2 = x2;
	this->mYPos2 = y2;
	this->mHeight = 75;
	this->mBase = 0;
	this->mVisited = false;
	this->mLeft = true;
	this->mTop = true;
	this->mRight = true;
	this->mBottom = true;
}

// Cell's Deconstructor //
Cell::~Cell(){}

// Returns the x1 coordinate of the cell (Bottom Left)//
double Cell::getXPos1(){ return this->mXPos1; }

// Returns the y1 coordinate of the cell (Bottom Left) //
double Cell::getYPos1(){ return this->mYPos1; }

// Returns the x2 coordinate of the cell (Top Right) //
double Cell::getXPos2(){ return this->mXPos2; }

// Returns the y2 coordinate of the cell (Top Right) //
double Cell::getYPos2(){ return this->mYPos2; }

// Set a specific wall to either existant or not //
void Cell::setWall(wall pos, bool exist)
{
	if(pos == W_LEFT)
		this->mLeft = exist;
	if(pos == W_TOP)
		this->mTop = exist;
	if(pos == W_RIGHT)
		this->mRight = exist;
	if(pos == W_BOTTOM)
		this->mBottom = exist;
}

// Get whether a specific wall exists or not //
bool Cell::getWallExists(wall pos)
{
	if(pos == W_LEFT)
		return this->mLeft;
	if(pos == W_TOP)
		return this->mTop;
	if(pos == W_RIGHT)
		return this->mRight;
	if(pos == W_BOTTOM)
		return this->mBottom;
}

// Set the cell to visited (Used in generating maze) //
void Cell::setVisisted(){ this->mVisited = true; }

// Get if the cell has been visited //
bool Cell::getVisited(){ return this->mVisited; }

// Draw the Cell to the Scene //
void Cell::drawCell(double thickness)
{	
	if(mLeft)
	{
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if(mXPos1 <= 10)
			glBindTexture(GL_TEXTURE_2D, 1);
		else
			glBindTexture(GL_TEXTURE_2D, 2);
		glBegin(GL_QUADS);
		drawWall(mXPos1,mYPos1,mXPos1,mYPos2,mHeight,mBase,thickness-2);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	if(mRight)
	{
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if(mXPos2 >= mXSize)
			glBindTexture(GL_TEXTURE_2D, 1);
		else
			glBindTexture(GL_TEXTURE_2D, 2);
		glBegin(GL_QUADS);
		drawWall(mXPos2,mYPos1,mXPos2,mYPos2,mHeight,mBase,thickness-2);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	if(mTop)
	{
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if(mYPos2 >= mYSize)
			glBindTexture(GL_TEXTURE_2D, 1);
		else
			glBindTexture(GL_TEXTURE_2D, 2);
		glBegin(GL_QUADS);
		drawWall(mXPos1,mYPos2,mXPos2,mYPos2,mHeight,mBase,thickness-2);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	if(mBottom)
	{
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if(mYPos1 <= 10)
			glBindTexture(GL_TEXTURE_2D, 1);
		else
			glBindTexture(GL_TEXTURE_2D, 2);
		glBegin(GL_QUADS);
		drawWall(mXPos1,mYPos1,mXPos2,mYPos1,mHeight,mBase,thickness-2);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	drawPosts(mXPos1,mYPos1,mXPos1,mYPos2,mHeight,mBase,thickness);
	drawPosts(mXPos2,mYPos1,mXPos2,mYPos2,mHeight,mBase,thickness);
	drawPosts(mXPos1,mYPos2,mXPos2,mYPos2,mHeight,mBase,thickness);
	drawPosts(mXPos1,mYPos1,mXPos2,mYPos1,mHeight,mBase,thickness);
}

// Draw a wall //
void Cell::drawWall(double x1, double y1, double x2, double y2, double height, double base, double thickness)
{
	double wallStart = -.5;
	double wallWidth = 1.5;
	double wallBase = -.25;
	double wallHeight = 1.25;
	//West Polygon
	glTexCoord2d(wallStart,wallBase);
	glVertex3d(x1-thickness/2,y1-thickness/2,base);
	glTexCoord2d(wallWidth,wallBase);
	glVertex3d(x1-thickness/2,y2+thickness/2,base);
	glTexCoord2d(wallWidth,wallHeight);
	glVertex3d(x1-thickness/2,y2+thickness/2,height);
	glTexCoord2d(wallStart,wallHeight);
	glVertex3d(x1-thickness/2,y1-thickness/2,height);

	//South Polygon
	glTexCoord2d(wallStart,wallBase);
	glVertex3d(x1-thickness/2,y1-thickness/2,base);
	glTexCoord2d(wallWidth,wallBase);
	glVertex3d(x2+thickness/2,y1-thickness/2,base);
	glTexCoord2d(wallWidth,wallHeight);
	glVertex3d(x2+thickness/2,y1-thickness/2,height);
	glTexCoord2d(wallStart,wallHeight);
	glVertex3d(x1-thickness/2,y1-thickness/2,height);

	//East Polygon
	glTexCoord2d(wallStart,wallBase);
	glVertex3d(x2+thickness/2,y1-thickness/2,base);
	glTexCoord2d(wallWidth,wallBase);
	glVertex3d(x2+thickness/2,y2+thickness/2,base);
	glTexCoord2d(wallWidth,wallHeight);
	glVertex3d(x2+thickness/2,y2+thickness/2,height);
	glTexCoord2d(wallStart,wallHeight);
	glVertex3d(x2+thickness/2,y1-thickness/2,height);

	//North Polygon
	glTexCoord2d(wallStart,wallBase);
	glVertex3d(x1-thickness/2,y2+thickness/2,base);
	glTexCoord2d(wallWidth,wallBase);
	glVertex3d(x2+thickness/2,y2+thickness/2,base);
	glTexCoord2d(wallWidth,wallHeight);
	glVertex3d(x2+thickness/2,y2+thickness/2,height);
	glTexCoord2d(wallStart,wallHeight);
	glVertex3d(x1-thickness/2,y2+thickness/2,height);

	//Top Polygon
	glTexCoord2d(0,0);
	glVertex3d(x1-thickness/2,y1-thickness/2,height);
	glTexCoord2d(1,0);
	glVertex3d(x2+thickness/2,y1-thickness/2,height);
	glTexCoord2d(1,.1);
	glVertex3d(x2+thickness/2,y2+thickness/2,height);
	glTexCoord2d(0,.1);
	glVertex3d(x1-thickness/2,y2+thickness/2,height);

	//Bottom Polygon
	glTexCoord2d(0,0);
	glVertex3d(x1-thickness/2,y1-thickness/2,base);
	glTexCoord2d(1,0);
	glVertex3d(x2+thickness/2,y1-thickness/2,base);
	glTexCoord2d(1,.1);
	glVertex3d(x2+thickness/2,y2+thickness/2,base);
	glTexCoord2d(0,.1);
	glVertex3d(x1-thickness/2,y2+thickness/2,base);
}

void Cell::drawCorner(double x1, double y1, double x2, double y2, double height, double base, double thickness)
{
	double postWidth = 1;
	double postHeight = 7.5;
	//West Polygon
	glTexCoord2d(0,0);
	glVertex3d(x1-thickness/2,y1-thickness/2,base);
	glTexCoord2d(postWidth,0);
	glVertex3d(x1-thickness/2,y2+thickness/2,base);
	glTexCoord2d(postWidth,postHeight);
	glVertex3d(x1-thickness/2,y2+thickness/2,height);
	glTexCoord2d(0,postHeight);
	glVertex3d(x1-thickness/2,y1-thickness/2,height);

	//South Polygon
	glTexCoord2d(0,0);
	glVertex3d(x1-thickness/2,y1-thickness/2,base);
	glTexCoord2d(postWidth,0);
	glVertex3d(x2+thickness/2,y1-thickness/2,base);
	glTexCoord2d(postWidth,postHeight);
	glVertex3d(x2+thickness/2,y1-thickness/2,height);
	glTexCoord2d(0,postHeight);
	glVertex3d(x1-thickness/2,y1-thickness/2,height);

	//East Polygon
	glTexCoord2d(0,0);
	glVertex3d(x2+thickness/2,y1-thickness/2,base);
	glTexCoord2d(postWidth,0);
	glVertex3d(x2+thickness/2,y2+thickness/2,base);
	glTexCoord2d(postWidth,postHeight);
	glVertex3d(x2+thickness/2,y2+thickness/2,height);
	glTexCoord2d(0,postHeight);
	glVertex3d(x2+thickness/2,y1-thickness/2,height);

	//North Polygon
	glTexCoord2d(0,0);
	glVertex3d(x1-thickness/2,y2+thickness/2,base);
	glTexCoord2d(postWidth,0);
	glVertex3d(x2+thickness/2,y2+thickness/2,base);
	glTexCoord2d(postWidth,postHeight);
	glVertex3d(x2+thickness/2,y2+thickness/2,height);
	glTexCoord2d(0,postHeight);
	glVertex3d(x1-thickness/2,y2+thickness/2,height);

	//Top Polygon
	glTexCoord2d(0,0);
	glVertex3d(x1-thickness/2,y1-thickness/2,height);
	glTexCoord2d(postWidth,0);
	glVertex3d(x2+thickness/2,y1-thickness/2,height);
	glTexCoord2d(postWidth,postWidth);
	glVertex3d(x2+thickness/2,y2+thickness/2,height);
	glTexCoord2d(0,postWidth);
	glVertex3d(x1-thickness/2,y2+thickness/2,height);

	//Bottom Polygon
	glTexCoord2d(0,0);
	glVertex3d(x1-thickness/2,y1-thickness/2,base);
	glTexCoord2d(postWidth,0);
	glVertex3d(x2+thickness/2,y1-thickness/2,base);
	glTexCoord2d(postWidth,postWidth);
	glVertex3d(x2+thickness/2,y2+thickness/2,base);
	glTexCoord2d(0,postWidth);
	glVertex3d(x1-thickness/2,y2+thickness/2,base);
}

// Draw the a Corner Post //
void Cell::drawPosts(double x1, double y1, double x2, double y2, double height, double base, double thickness)
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 3);
	glBegin(GL_QUADS);
	drawCorner(x1,y1,x1,y1,height,base,thickness);
	drawCorner(x2,y2,x2,y2,height,base,thickness);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}