#include <cmath>
#include <iostream>
#include <cstring>
#include <time.h>
#include "maze.h"
#include "player.h"
#include "bullet.h"
#include "glut.h"
#include "helpers.h"
#include "sceneManager.h"

double screen_x = 920;
double screen_y = 620;

// Maze Globals
int X_SIZE = 9;
int Y_SIZE = 6;
int CELL_SIZE = 100;
int OUTER_BORDER = 10;
double PLAYER_SCALE = 2;
double WALL_THICKNESS = 8;
Maze * MAZE;

// Tank Globals
Player * PLAYER;
Bullet * BULLET;
bool FIRED = false;

// Other Globals
int KEYS[6] = {0,0,0,0,0,0}; //{Forward,Backward,RotateRight,RotateLeft,RotateTurretRight,RotateTurretLeft}
double CURRENT_TIME, LAST_TIME, DELTA_TIME, DELTA_TIME_SECONDS;
int VIEW_MODE = 1; //{ 0 = Top Dow, 1 = Perspective, 2 = First Person)
int MODEL_MODE = 0; //{ 0 = Solid, 1 = Wireframe, 2 = Points}
int SHADING = 0; //{ 0 = Smooth, 1 = Flat }
SceneManager RENDER;

void drawWorld()
{
	glColor3d(.076,.022,.075);
	glBegin(GL_QUADS);
	glVertex3d(-screen_x,-screen_y,-.1);
	glVertex3d(screen_x*2,-screen_y,-.1);
	glColor3d(.076,.022,.0075);
	glVertex3d(screen_x*2,screen_y*2,-.1);
	glColor3d(.376,.22,.075);
	glVertex3d(-screen_x,screen_y*2,-.1);
	glEnd();

	glColor3d(.22,.58,1);
	glBegin(GL_QUADS);
	glVertex3d(-screen_x,-screen_y,0);
	glVertex3d(-screen_x,screen_y*2,0);
	glColor3d(.65,.81,1);
	glVertex3d(-screen_x,screen_y*2,screen_y*2);
	glVertex3d(-screen_x,-screen_y,screen_y*2);
	glEnd();

	glColor3d(.22,.58,1);
	glBegin(GL_QUADS);
	glVertex3d(screen_x*2,-screen_y,0);
	glVertex3d(screen_x*2,screen_y*2,0);
	glColor3d(.65,.81,1);
	glVertex3d(screen_x*2,screen_y*2,screen_y*2);
	glVertex3d(screen_x*2,-screen_y,screen_y*2);
	glEnd();

	glColor3d(.22,.58,1);
	glBegin(GL_QUADS);
	glVertex3d(-screen_x,-screen_y,0);
	glVertex3d(screen_x*2,-screen_y,0);
	glColor3d(.65,.81,1);
	glVertex3d(screen_x*2,-screen_y,screen_y*2);
	glVertex3d(-screen_x,-screen_y,screen_y*2);
	glEnd();

	glColor3d(.22,.58,1);
	glBegin(GL_QUADS);
	glVertex3d(-screen_x,screen_y*2,0);
	glVertex3d(screen_x*2,screen_y*2,0);
	glColor3d(.65,.81,1);
	glVertex3d(screen_x*2,screen_y*2,screen_y*2);
	glVertex3d(-screen_x,screen_y*2,screen_y*2);
	glEnd();

}

void display(void)
{
	if(VIEW_MODE == 0)
		SetOrthographicProjection(screen_x,screen_y);
	else if(VIEW_MODE == 1 || VIEW_MODE == 2)
		SetPerspectiveProjection(screen_x,screen_y);
	else if(VIEW_MODE == 3)
		Set3rdPersonProjection(screen_x,screen_y);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	CURRENT_TIME = clock();
	DELTA_TIME = CURRENT_TIME - LAST_TIME;
	LAST_TIME = CURRENT_TIME;
	DELTA_TIME_SECONDS = DELTA_TIME/1000.0;

	if(VIEW_MODE == 1)
	{
		glEnable(GL_DEPTH_TEST);
		double eyeX = screen_x/2;
		double eyeY = -screen_y/2;
		double eyeZ = screen_y;
		double atX = screen_x/2;
		double atY = screen_y/2; 
		double atZ = 0;
		double upX = 0;
		double upY = 0;
		double upZ = 1;
		gluLookAt(eyeX,eyeY,eyeZ,atX,atY,atZ,upX,upY,upZ);
	}
	else if(VIEW_MODE == 2)
	{
		glEnable(GL_DEPTH_TEST);
		gluLookAt(PLAYER->getXPos(),PLAYER->getYPos(),15*PLAYER_SCALE,PLAYER->getCannonX(),PLAYER->getCannonY(),7.5*PLAYER_SCALE,0,0,1);	
	}
	else if(VIEW_MODE == 3)
	{
		glEnable(GL_DEPTH_TEST);
		gluLookAt(PLAYER->getCannon3PersonViewX(),PLAYER->getCannon3PersonViewY(),20*PLAYER_SCALE,PLAYER->getCannonX(),PLAYER->getCannonY(),7.5*PLAYER_SCALE,0,0,1);	
	}
	// Background

	drawWorld();

	MAZE->drawMaze(WALL_THICKNESS);
	PLAYER->drawPlayer(&RENDER,DELTA_TIME_SECONDS,MODEL_MODE);
	BULLET->drawBullet();
	BULLET->drawExplosion(DELTA_TIME_SECONDS,CELL_SIZE);

	glutSwapBuffers();
	glutPostRedisplay();
	glFlush();
}

void SetOrthographicProjection(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h,-50,50);
	glMatrixMode(GL_MODELVIEW);
}

void SetPerspectiveProjection(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double fov = 60;
	double aspectRatio = (double)w/h;
	double zNear = 2.0;
	double zFar = (w+h)*2;
	gluPerspective(fov,aspectRatio,zNear,zFar);
	glMatrixMode(GL_MODELVIEW);
}

void Set3rdPersonProjection(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double fov = 60;
	double aspectRatio = (double)w/h;
	double zNear = 20;
	double zFar = (w+h)*2;
	gluPerspective(fov,aspectRatio,zNear,zFar);
	glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;
	glViewport(0, 0, w, h);
	if(VIEW_MODE == 0)
		SetOrthographicProjection(w,h);
	else if(VIEW_MODE == 1)
		SetPerspectiveProjection(w,h);
}

void InitializeMyStuff()
{
	srand(time(0));
	RENDER.initializeTextures();
	RENDER.loadModel("tankBase.obj");
	RENDER.loadModel("tankCannon.obj");
	cout << RENDER.getPolygonCount();
	BULLET = new Bullet(PLAYER_SCALE);
	PLAYER = new Player(X_SIZE,Y_SIZE,CELL_SIZE,OUTER_BORDER,PLAYER_SCALE);
	MAZE = new Maze(X_SIZE,Y_SIZE,CELL_SIZE,OUTER_BORDER,WALL_THICKNESS);
	LAST_TIME = clock();
}

int main(int argc, char **argv)
{
	printInstructions();
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Maze!");
	}

	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutKeyboardFunc(keysPressed);
	glutKeyboardUpFunc(keysUp);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(0, 0, 0, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}

void keysPressed(unsigned char key, int x, int y)
{
	if(key == 'p')
	{
		VIEW_MODE = 1;
	}
	if(key == 'o')
	{
		VIEW_MODE = 0;
	}
	if(key == 'l')
	{
		VIEW_MODE = 2;
	}
	if(key == 'k')
	{
		VIEW_MODE = 3;
	}
	if(key == 't')
	{
		MODEL_MODE = 0;
	}
	if(key == 'y')
	{
		MODEL_MODE = 1;
	}
	if(key == 'u')
	{
		MODEL_MODE = 2;
	}
	if(key == 'c')
	{
		if(PLAYER->getCollisionDetection())
			PLAYER->setCollisionDetection(false);
		else
			PLAYER->setCollisionDetection(true);
	}
	if(key == 'w')
		KEYS[0] = 1;
	if(key == 's')
		KEYS[1] = 1;
	if(key == 'd')
		KEYS[2] = 1;
	if(key == 'a')
		KEYS[3] = 1;
	if(key == 'e')
		KEYS[4] = 1;
	if(key == 'q')
		KEYS[5] = 1;
	if(key == 32 && !FIRED)
	{
		FIRED = true;
		PLAYER->fireCannon();
		BULLET->setBulletStartPos(PLAYER->getCannonX(),PLAYER->getCannonY());
		BULLET->setBulletDegrees(PLAYER->getCannonDegrees());
		BULLET->fireBullet();
	}

}	

void keysUp(unsigned char key, int x, int y)
{
	if(key == 27)
		exit(0);
	if(key == 'w')
		KEYS[0] = 0;
	if(key == 's')
		KEYS[1] = 0;
	if(key == 'd')
		KEYS[2] = 0;
	if(key == 'a')
		KEYS[3] = 0;
	if(key == 'e')
		KEYS[4] = 0;
	if(key == 'q')
		KEYS[5] = 0;
	if(key == 32)
		FIRED = false;
}

void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

void update()
{
	if(KEYS[0] == 1)
		PLAYER->movePlayer(Player::D_FORWARD,DELTA_TIME_SECONDS,MAZE);
	if(KEYS[1] == 1)
		PLAYER->movePlayer(Player::D_BACKWARD,DELTA_TIME_SECONDS,MAZE);
	if(KEYS[2] == 1)
		PLAYER->rotatePlayer(Player::D_RIGHT,DELTA_TIME_SECONDS);
	if(KEYS[3] == 1)
		PLAYER->rotatePlayer(Player::D_LEFT,DELTA_TIME_SECONDS);
	if(KEYS[4] == 1)
		PLAYER->rotateTurret(Player::D_RIGHT,DELTA_TIME_SECONDS);
	if(KEYS[5] == 1)
		PLAYER->rotateTurret(Player::D_LEFT,DELTA_TIME_SECONDS);
	BULLET->moveBullet(MAZE,DELTA_TIME_SECONDS);
}

void printInstructions()
{
	cout << "Welcome to the Tank Maze" << endl;
	cout << "By: Darren Kent" << endl << endl;
	cout << "Controls:" << endl;
	cout << "Forward:\t\t\tw" << endl;
	cout << "Backwards:\t\t\ts" << endl;
	cout << "Rotate Right:\t\t\td" << endl;
	cout << "Rotate Left:\t\t\ta" << endl;
	cout << "Rotate Cannon Right:\t\te" << endl;
	cout << "Rotate Cannon Left:\t\tq" << endl;
	cout << "Fire Cannon:\t\t\tSpace" << endl << endl;
	cout << "Orthographic View:\t\to"<< endl;
	cout << "Perspective View: \t\tp"<< endl;
	cout << "First Person View: \t\tl"<< endl;
	cout << "Third Person View: \t\tk" << endl << endl;
	cout << "Tank Solid Mode: \t\tt" << endl;
	cout << "Tank Wireframe Mode: \t\t\y" << endl;
	cout << "Tank Vertex Mode: \t\tu" << endl;
	cout << "Collision Bounding Box: \tc" << endl;
	system("PAUSE");
}