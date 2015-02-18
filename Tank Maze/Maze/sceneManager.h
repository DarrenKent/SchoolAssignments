#ifndef _PARSER_
#define _PARSER_

#include <string>
#include <vector>
#include <conio.h>
#include "glew\include\GL\glew.h"
using namespace std;
#include "Tga.h"
#include "model.h"
#include "texture.h"

class SceneManager{
public:
	SceneManager();
	~SceneManager();

	int getPolygonCount();
	void drawAllModels(int mode);
	void drawModel(string name, int mode);
	void loadModel(string filename);
	Model* getModel(string name);
	bool parseFile(string filename);
	void initializeTextures();
	gliGenericImage * readImage(char *filename);
	gliGenericImage * readTgaImage(char *filename);
	void SetBorder(gliGenericImage * image);
	bool PowerOf2(int h);
protected:
private:
	vector<Model*> mModels;
	vector<Texture*> mTextures;
	int mPolygonCount;
	static const int mNumTextures = 3;
	GLuint texName[mNumTextures];
};

#endif "_PARSER_"