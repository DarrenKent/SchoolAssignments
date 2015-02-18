#ifndef _CELL_
#define _CELL_


class Cell
{
public:
	enum wall{
		W_LEFT = 0,
		W_TOP,
		W_RIGHT,
		W_BOTTOM
	};

	Cell(double x1, double y1, double x2, double y2, int xsize, int ysize);
	~Cell();

	double getXPos1();
	double getYPos1();
	double getXPos2();
	double getYPos2();

	void setWall(wall pos, bool exist);
	bool getWallExists(wall pos);

	void setVisisted();
	bool getVisited();

	void drawCell(double thickness);
	void drawWall(double x1, double y1, double x2, double y2, double height, double base, double thickness);
	void drawCorner(double x1, double y1, double x2, double y2, double height, double base, double thickness);
	void drawPosts(double x1, double y1, double x2, double y2, double height, double base, double thickness);

protected:
private:
	double mXPos1,mYPos1,mXPos2,mYPos2,mHeight,mBase,mXSize,mYSize;
	bool mLeft,mTop,mRight,mBottom,mVisited,mOuterWall;

};
#endif "_CELL_"