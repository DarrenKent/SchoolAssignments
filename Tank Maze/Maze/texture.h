#ifndef _TEXTURE_
#define _TEXTURE_

#include <string>

class Texture{
public:
	Texture(string filename){ mTexName = filename; mRepeat = false; mBorder = false; }

	bool mRepeat,mBorder;
	string mTexName;
};

#endif _TEXTURE_