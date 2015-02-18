#ifndef _MODEL_
#define _MODEL_
#include <string>
#include <vector>

using namespace std;

class Model
{
public:
	struct vert
	{
		double vX;
		double vY;
		double vZ;
	};
	struct triangle
	{
		double p1;
		double p2;
		double p3;
		double cR;
		double cG;
		double cB;
	};

	Model(double x, double y, double z, string name);
	~Model();

	void setPosition(double x, double y, double z);
	void drawObject(int mode);
	void addVertice(vert vertice);
	void addTriangle(triangle tri);

	string getId();

	void createTestCube();
protected:
private:
	double mXPos,mYPos,mZPos;
	double mScale, mShader;
	std::vector<vert> mVertices;
	std::vector<triangle> mPolygons;
	string mId;
};
#endif "_MODEL_"