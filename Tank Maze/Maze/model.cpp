#include <iostream>
#include "model.h"
#include "glut.h"

Model::Model(double x, double y, double z, string name)
{
	mXPos = x;
	mYPos = y;
	mZPos = z;
	mScale = .1;
	mId = name;
	mShader = .2;
}

Model::~Model(){}

void Model::setPosition(double x, double y, double z)
{
	mXPos = x;
	mYPos = y;
	mZPos = z;
}

string Model::getId()
{
	return mId;
}

void Model::drawObject(int mode)
{

	glPushMatrix();
	glTranslated(mXPos,mYPos,mZPos);
	
	if(mode == 0)
		glBegin(GL_TRIANGLES);
	else if(mode ==1)
		glBegin(GL_LINES);
	else if(mode ==2)
		glBegin(GL_POINTS);

	for(int i=0;i < mPolygons.size();i++)
	{	
		glColor3d(mPolygons[i].cR,mPolygons[i].cG,mPolygons[i].cB);
		glVertex3d(mVertices[mPolygons[i].p1].vX*mScale,mVertices[mPolygons[i].p1].vY*mScale,mVertices[mPolygons[i].p1].vZ*mScale);
		glColor3d(mPolygons[i].cR-mShader,mPolygons[i].cG-mShader,mPolygons[i].cB-mShader);
		glVertex3d(mVertices[mPolygons[i].p2].vX*mScale,mVertices[mPolygons[i].p2].vY*mScale,mVertices[mPolygons[i].p2].vZ*mScale);
		if(mode == 1)
			glVertex3d(mVertices[mPolygons[i].p2].vX*mScale,mVertices[mPolygons[i].p2].vY*mScale,mVertices[mPolygons[i].p2].vZ*mScale);
		glColor3d(mPolygons[i].cR,mPolygons[i].cG,mPolygons[i].cB);
		glVertex3d(mVertices[mPolygons[i].p3].vX*mScale,mVertices[mPolygons[i].p3].vY*mScale,mVertices[mPolygons[i].p3].vZ*mScale);
		if(mode == 1)
		{
			glVertex3d(mVertices[mPolygons[i].p3].vX*mScale,mVertices[mPolygons[i].p3].vY*mScale,mVertices[mPolygons[i].p3].vZ*mScale);
			glColor3d(mPolygons[i].cR,mPolygons[i].cG,mPolygons[i].cB);
			glVertex3d(mVertices[mPolygons[i].p1].vX*mScale,mVertices[mPolygons[i].p1].vY*mScale,mVertices[mPolygons[i].p1].vZ*mScale);
		}
	}
	glEnd();
	glPopMatrix();
}

void Model::addTriangle(triangle tri)
{
	mPolygons.push_back(tri);
}

void Model::addVertice(vert vertice)
{
	mVertices.push_back(vertice);
}

void Model::createTestCube()
{
	vert temp1;
	temp1.vX = -21.9373;
	temp1.vY = 0.0000;
	temp1.vZ = 21.3675;

	vert temp2;
	temp2.vX = -21.9373;
	temp2.vY = 0.0000;
	temp2.vZ = -16.2393;

	vert temp3;
	temp3.vX = 37.3219;
	temp3.vY = 0.0000;
	temp3.vZ = -16.2393;

	vert temp4;
	temp4.vX = 37.3219;
	temp4.vY = 0.0000;
	temp4.vZ = 21.3675;

	vert temp5;
	temp5.vX = -21.9373;
	temp5.vY = 14.2450;
	temp5.vZ = 21.3675;

	vert temp6;
	temp6.vX = 37.3219;
	temp6.vY = 14.2450;
	temp6.vZ = 21.3675;

	vert temp7;
	temp7.vX = 37.3219;
	temp7.vY = 14.2450;
	temp7.vZ = -16.2393;

	vert temp8;
	temp8.vX = -21.9373;
	temp8.vY = 14.2450;
	temp8.vZ = -16.2393;

	mVertices.push_back(temp1);
	mVertices.push_back(temp2);
	mVertices.push_back(temp3);
	mVertices.push_back(temp4);
	mVertices.push_back(temp5);
	mVertices.push_back(temp6);
	mVertices.push_back(temp7);
	mVertices.push_back(temp8);

	triangle qTemp1;
	qTemp1.p1 = 0;
	qTemp1.p2 = 1;
	qTemp1.p3 = 2;
	qTemp1.cR = 1;
	qTemp1.cB = 0;
	qTemp1.cG = 0;

	triangle qTemp2;
	qTemp2.p1 = 2;
	qTemp2.p2 = 3;
	qTemp2.p3 = 0;
	qTemp2.cR = 1;
	qTemp2.cB = 0;
	qTemp2.cG = 0;

	triangle qTemp3;
	qTemp3.p1 = 4;
	qTemp3.p2 = 5;
	qTemp3.p3 = 6;
	qTemp3.cR = 1;
	qTemp3.cB = 0;
	qTemp3.cG = 0;

	triangle qTemp4;
	qTemp4.p1 = 6;
	qTemp4.p2 = 7;
	qTemp4.p3 = 4;
	qTemp4.cR = 1;
	qTemp4.cB = 0;
	qTemp4.cG = 0;

	triangle qTemp5;
	qTemp5.p1 = 0;
	qTemp5.p2 = 3;
	qTemp5.p3 = 5;
	qTemp5.cR = 1;
	qTemp5.cB = 0;
	qTemp5.cG = 0;

	triangle qTemp6;
	qTemp6.p1 = 5;
	qTemp6.p2 = 4;
	qTemp6.p3 = 0;
	qTemp6.cR = 1;
	qTemp6.cB = 0;
	qTemp6.cG = 0;

	triangle qTemp7;
	qTemp7.p1 = 3;
	qTemp7.p2 = 2;
	qTemp7.p3 = 6;
	qTemp7.cR = 1;
	qTemp7.cB = 0;
	qTemp7.cG = 0;

	triangle qTemp8;
	qTemp8.p1 = 6;
	qTemp8.p2 = 5;
	qTemp8.p3 = 2;
	qTemp8.cR = 1;
	qTemp8.cB = 0;
	qTemp8.cG = 0;

	triangle qTemp9;
	qTemp9.p1 = 2;
	qTemp9.p2 = 1;
	qTemp9.p3 = 7;
	qTemp9.cR = 1;
	qTemp9.cB = 0;
	qTemp9.cG = 0;

	triangle qTemp10;
	qTemp10.p1 = 7;
	qTemp10.p2 = 6;
	qTemp10.p3 = 2;
	qTemp10.cR = 1;
	qTemp10.cB = 0;
	qTemp10.cG = 0;

	triangle qTemp11;
	qTemp11.p1 = 1;
	qTemp11.p2 = 0;
	qTemp11.p3 = 4;
	qTemp11.cR = 1;
	qTemp11.cB = 0;
	qTemp11.cG = 1;

	triangle qTemp12;
	qTemp12.p1 = 4;
	qTemp12.p2 = 7;
	qTemp12.p3 = 1;
	qTemp12.cR = 1;
	qTemp12.cB = 1;
	qTemp12.cG = 0;

	mPolygons.push_back(qTemp1);
	mPolygons.push_back(qTemp2);
	mPolygons.push_back(qTemp3);
	mPolygons.push_back(qTemp4);
	mPolygons.push_back(qTemp5);
	mPolygons.push_back(qTemp6);
	mPolygons.push_back(qTemp7);
	mPolygons.push_back(qTemp8);
	mPolygons.push_back(qTemp9);
	mPolygons.push_back(qTemp10);
	mPolygons.push_back(qTemp11);
	mPolygons.push_back(qTemp12);
}