#ifndef _MAZE_
#define _MAZE_

#include <vector>
using namespace std;
#include "cell.h"


class Maze
{
public:
	Maze(int xSize, int ySize, int cellSize, int outerBorder, int thickness);
	~Maze();

	void populateMaze();
	void resetMaze();
	void createMaze();
	void recurseCreateMaze(int cellNum);
	

	void drawMaze(double thickness);
	
	Cell* getCell(int cellNum);

	bool isSafe(double x, double y, double radius);

	void destroyWallAt(double x, double y);
protected:
private:
	int mXSize, mYSize, mThickness,mCellSize,mOuterBorder;
	vector<Cell*> mMazeMap;
};


#endif "_MAZE_"