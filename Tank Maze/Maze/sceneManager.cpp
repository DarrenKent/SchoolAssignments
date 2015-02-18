#include <fstream>
#include <sstream>
#include <iostream>
#include "sceneManager.h"

SceneManager::SceneManager()
{
	mPolygonCount = 0;
	Texture * brick = new Texture("brick.tga");
	Texture * target = new Texture("target.tga");
	Texture * metal = new Texture("metal.tga");
	brick->mRepeat = true;
	target->mBorder = true;
	metal->mRepeat = true;
	mTextures.push_back(brick);
	mTextures.push_back(target);
	mTextures.push_back(metal);
}

SceneManager::~SceneManager(){}

int SceneManager::getPolygonCount(){ return mPolygonCount; }

void SceneManager::loadModel(string filename)
{
	bool success = parseFile(filename);
	if(!success)
	{
		cout << "Error Opening File. Cannot find file: " << filename << endl;
		exit(0);
	}
}

void SceneManager::drawAllModels(int mode)
{
	for(int i=0;i<mModels.size();i++)
	{
		mModels[i]->drawObject(mode);
	}
}

void SceneManager::drawModel(string name,int mode)
{
	for(int i=0;i < mModels.size();i++)
	{
		if(name == mModels[i]->getId())
			mModels[i]->drawObject(mode);
	}
}

Model * SceneManager::getModel(string name)
{
	for(int i=0; i < mModels.size();i++)
	{
		if(name == mModels[i]->getId())
			return mModels[i];
	}
}

bool SceneManager::parseFile(string filename)
{
	Model* model = new Model(0,0,0,filename);
	int count = 0;
	string line;
	ifstream file (filename);
	if(file.is_open())
	{
		double r,g,b;
		while(file.good())
		{
			getline (file,line);
			string token;
			istringstream ss(line);

			getline(ss,token,' ');
			if(token == "v")
			{
				Model::vert temp;
				getline(ss,token,' ');
				getline(ss,token,' ');
				temp.vX = atof(token.c_str());
				getline(ss,token,' ');
				temp.vY = atof(token.c_str());
				getline(ss,token,' ');
				temp.vZ = atof(token.c_str());
				model->addVertice(temp);
			}
			else if(token == "f")
			{
				Model::triangle temp;
				getline(ss,token,' ');
				temp.p1 = atof(token.c_str())-1;
				getline(ss,token,' ');
				temp.p2 = atof(token.c_str())-1;
				getline(ss,token,' ');
				temp.p3 = atof(token.c_str())-1;
				temp.cR = r;
				temp.cG = g;
				temp.cB = b;
				model->addTriangle(temp);
				mPolygonCount ++;
			}
			else if(token == "usemtl")
			{
				getline(ss,token,' ');
				string red;
				red = token[5];
				red += token[6];
				red += token[7];
				string green;
				green = token[8];
				green += token[9];
				green += token[10];
				string blue;
				blue = token[11];
				blue += token[12];
				blue += token[13];
				r = atof(red.c_str())/255;
				g = atof(green.c_str())/255;
				b = atof(blue.c_str())/255;

			}
		}
		file.close();
		mModels.push_back(model);
		return true;
	}
	else
		return false;
}

void SceneManager::initializeTextures()
{
	gliGenericImage *image[mNumTextures];
	int n = 0;

	for(unsigned int i = 0;i<mTextures.size();i++)
	{
		char * line = new char[mTextures[i]->mTexName.size()+1];
		std::copy(mTextures[i]->mTexName.begin(),mTextures[i]->mTexName.end(),line);
		line[mTextures[i]->mTexName.size()] = '\0';
		image[n++] = readImage(line);
	}

	if(n!=mNumTextures)
	{
		printf("Error: Wrong number of textures\n");
		_getch();
		exit(1);;
	}

	glGenTextures(mNumTextures, texName);

	for(int i=0; i<mNumTextures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		int repeats = mTextures[i]->mRepeat;
		int needs_border = mTextures[i]->mBorder; // Needed if clamping and not filling the whole polygon.
		if(repeats)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}
		if(needs_border)
		{
			// set a border.
			//SetBorder(image[i]);
			float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color );
		}

		bool mipmaps = false;
		if(!PowerOf2(image[i]->height) || !PowerOf2(image[i]->width))
		{
			// WARNING: Images that do not have width and height as 
			// powers of 2 MUST use mipmaps.
			mipmaps = true; 
		}

		if (mipmaps) 
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, image[i]->components,
					image[i]->width, image[i]->height,
					image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					//GL_LINEAR_MIPMAP_LINEAR);
					GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
					//GL_LINEAR);
					GL_NEAREST);
		} 
		else 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, image[i]->components,
					image[i]->width, image[i]->height, 0,
					image[i]->format, GL_UNSIGNED_BYTE, image[i]->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}
}


gliGenericImage * SceneManager::readImage(char *filename)
{
	size_t size = strlen(filename);
	if(toupper(filename[size-3]) == 'T' && toupper(filename[size-2]) == 'G' && toupper(filename[size-1]) == 'A')
	{
		gliGenericImage * result = readTgaImage(filename);
		if(!result)
		{
			cerr << "Error opening " << filename << endl;
			_getch();
			exit(1);
		}
		return result;
	}
	else
	{
		cerr << "Unknown Filetype!\n";
		_getch();
		exit(1);
	}
}

gliGenericImage * SceneManager::readTgaImage(char *filename)
{
  FILE *file;
  gliGenericImage *image;

  file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Error: could not open \"%s\"\n", filename);
    return NULL;
  }
  image = gliReadTGA(file, filename);
  fclose(file);
  if (image == NULL) {
    printf("Error: could not decode file format of \"%s\"\n", filename);
    return NULL;
  }
  return image;
}

void SceneManager::SetBorder(gliGenericImage * image)
{
	// set a border color.
	unsigned int border_r=50;
	unsigned int border_g=50;
	unsigned int border_b=255;
	int x,y;
	y=0;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=1;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=image->height-1;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	y=image->height-2;
	for(x=0; x<image->width; x++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}

	x=0;
	for(y=0; y<image->height; y++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	x=1;
	for(y=0; y<image->height; y++)
	{
		image->pixels[ 3*(y*image->width + x) + 0]=border_r;
		image->pixels[ 3*(y*image->width + x) + 1]=border_g;
		image->pixels[ 3*(y*image->width + x) + 2]=border_b;
	}
	x=image->width-1;
	for(y=0; y<image->height; y++)
	{
		int index = 3*(y*image->width + x);
		image->pixels[ index + 0]=border_r;
		image->pixels[ index + 1]=border_g;
		image->pixels[ index + 2]=border_b;
	}
	x=image->width-2;
	for(y=0; y<image->height; y++)
	{
		int index = 3*(y*image->width + x);
		image->pixels[ index + 0]=border_r;
		image->pixels[ index + 1]=border_g;
		image->pixels[ index + 2]=border_b;
	}
}

bool SceneManager::PowerOf2(int h)
{
	if(h!= 2 && h!=4 && h!=8 && h!=16 && h!=32 && h!=64 && h!=128 && 
				h!=256 && h!=512 && h!=1024 && h!=2048 && h!=4096)
		return false;
	else
		return true;
}