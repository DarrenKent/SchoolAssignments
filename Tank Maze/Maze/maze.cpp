#include <iostream>
#include "maze.h"
#include "glut.h"


// Maze Constructor //
Maze::Maze(int xSize, int ySize, int cellSize, int outerBorder, int thickness)
{
	this->mThickness = thickness;
	this->mXSize = xSize;
	this->mYSize = ySize;
	this->mCellSize = cellSize;
	this->mOuterBorder = outerBorder;
	populateMaze();
	createMaze();
}

// Maze DeConstructor //
Maze::~Maze(){}

// Places Cells with all 4 Walls into the Maze //
void Maze::populateMaze()
{
	for(int i=0;i<mYSize;i++)
	{
		for(int h=0;h<mXSize;h++)
		{
			Cell * tempCell = new Cell(mOuterBorder+h*mCellSize,mOuterBorder+i*mCellSize,mOuterBorder+h*mCellSize+mCellSize,mOuterBorder+i*mCellSize+mCellSize,mXSize*mCellSize,mYSize*mCellSize);
			this->mMazeMap.push_back(tempCell);
		}
	}
}

// Clear last Maze and Generate New one //
void Maze::resetMaze()
{
	mMazeMap.clear();
	populateMaze();
	createMaze();
}

// Start Creating the Maze //
void Maze::createMaze()
{
	//mMazeMap[0]->setWall(Cell::W_LEFT,false);
	recurseCreateMaze(0);
	mMazeMap[mMazeMap.size()-1]->setWall(Cell::W_RIGHT,false);
}

// Recurse through the Maze and Generate Path //
void Maze::recurseCreateMaze(int cellNum)
{
	mMazeMap[cellNum]->setVisisted();
	int cells[4] = {-1,-1,-1,-1};
	if(cellNum % mXSize != 0)
		cells[0] = cellNum-1;
	if(cellNum+mXSize < mXSize*mYSize-1)
		cells[1] = cellNum+mXSize;
	if(cellNum % mXSize != mXSize-1)
		cells[2] = cellNum+1;
	if(cellNum-mXSize >= 0)
		cells[3] = cellNum-mXSize;
	int cellDir[4];
	for(int i=0;i<4;i++)
	{
		cellDir[i] = cells[i];
	}
	for(int i=0;i<4;i++)
	{
		int rCell = rand()%4;
		int rCell2 = rand()%4;
		int tempCell1 = cells[rCell];
		int tempCell2 = cells[rCell2];
		cells[rCell] = tempCell2;
		cells[rCell2] = tempCell1;
	}
	for(int h=0;h<4;h++)
	{
		if(cells[h]>0 && !mMazeMap[cells[h]]->getVisited())
		{
			for(int i=0;i<4;i++)
			{
				if(cellDir[i] == cells[h])
				{
					if(i == Cell::W_LEFT)
					{
						mMazeMap[cellNum]->setWall(Cell::W_LEFT,false);
						mMazeMap[cells[h]]->setWall(Cell::W_RIGHT,false);
						recurseCreateMaze(cells[h]);
						break;
					}
					if(i == Cell::W_TOP)
					{
						mMazeMap[cellNum]->setWall(Cell::W_TOP,false);
						mMazeMap[cells[h]]->setWall(Cell::W_BOTTOM,false);
						recurseCreateMaze(cells[h]);
						break;
					}
					if(i == Cell::W_RIGHT)
					{
						mMazeMap[cellNum]->setWall(Cell::W_RIGHT,false);
						mMazeMap[cells[h]]->setWall(Cell::W_LEFT,false);
						recurseCreateMaze(cells[h]);
						break;
					}
					if(i == Cell::W_BOTTOM)
					{
						mMazeMap[cellNum]->setWall(Cell::W_BOTTOM,false);
						mMazeMap[cells[h]]->setWall(Cell::W_TOP,false);
						recurseCreateMaze(cells[h]);
						break;
					}
				}
			}
		}
	}
	return;
}

// Draw The Maze to the Scene //
void Maze::drawMaze(double thickness)
{
	for(int i=0;i<mMazeMap.size();i++)
	{
		mMazeMap[i]->drawCell(thickness);
	}
}

// Grab a specific cell from the Vector //
Cell* Maze::getCell(int cellNum)
{
	return mMazeMap[cellNum];
}

// Determine if a Collsion will Occur to a cell Wall //
bool Maze::isSafe(double x, double y, double radius)
{
	if(x > mOuterBorder && x < mXSize*mCellSize+mOuterBorder && y > mOuterBorder && y < mYSize*mCellSize+mOuterBorder)
	{
		int cellX = (int)x;
		int cellY = (int)y;
		int cellCoord = (mXSize*((int)(y-mOuterBorder)/mCellSize) + (int)((x-mOuterBorder)/mCellSize));
		if(cellY-((cellY-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder-radius < 0 && mMazeMap[cellCoord]->getWallExists(Cell::W_BOTTOM))
			return true;
		if(cellY-((cellY-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder+radius > mCellSize && mMazeMap[cellCoord]->getWallExists(Cell::W_TOP))
			return true;
		if(cellX-((cellX-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder-radius < 0 && mMazeMap[cellCoord]->getWallExists(Cell::W_LEFT))
			return true;
		if(cellX-((cellX-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder+radius > mCellSize && mMazeMap[cellCoord]->getWallExists(Cell::W_RIGHT))
			return true;
	
		if(cellY-((cellY-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder-radius < 0 && cellX-((cellX-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder-radius < 0)
			return true;
		if(cellY-((cellY-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder+radius > mCellSize && cellX-((cellX-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder-radius < 0)
			return true;
		if(cellY-((cellY-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder+radius > mCellSize && cellX-((cellX-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder+radius > mCellSize)
			return true;
		if(cellY-((cellY-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder-radius < 0 && cellX-((cellX-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder+radius > mCellSize)
			return true;
	}

	return false;
}

// Destroy a specific wall based on an x and y value //
void Maze::destroyWallAt(double x, double y)
{
	int cellX = (int)x;
	int cellY = (int)y;
	int cellCoord = (mXSize*((int)(y-mOuterBorder)/mCellSize) + (int)((x-mOuterBorder)/mCellSize));
	if(cellY-((cellY-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder-10 < 0 && mMazeMap[cellCoord]->getWallExists(Cell::W_BOTTOM))
	{
		if(cellCoord >= mXSize)
		{
			mMazeMap[cellCoord]->setWall(Cell::W_BOTTOM,false);
			mMazeMap[cellCoord-mXSize]->setWall(Cell::W_TOP,false);
		}
	}
	if(cellY-((cellY-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder+10 > mCellSize && mMazeMap[cellCoord]->getWallExists(Cell::W_TOP))
	{
		if(cellCoord <= (mYSize*mXSize-1)-mXSize)
		{
			mMazeMap[cellCoord]->setWall(Cell::W_TOP,false);
			mMazeMap[cellCoord+mXSize]->setWall(Cell::W_BOTTOM,false);
		}
	}
	if(cellX-((cellX-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder-10 < 0 && mMazeMap[cellCoord]->getWallExists(Cell::W_LEFT))
	{
		if(cellCoord%mXSize != 0)
		{
			mMazeMap[cellCoord]->setWall(Cell::W_LEFT,false);
			mMazeMap[cellCoord-1]->setWall(Cell::W_RIGHT,false);
		}
	}
	if(cellX-((cellX-mOuterBorder)/mCellSize)*mCellSize-mOuterBorder+10 > mCellSize && mMazeMap[cellCoord]->getWallExists(Cell::W_RIGHT))
	{
		if(cellCoord%mXSize != mXSize-1)
		{
			mMazeMap[cellCoord]->setWall(Cell::W_RIGHT,false);
			mMazeMap[cellCoord+1]->setWall(Cell::W_LEFT,false);
		}
	}
}