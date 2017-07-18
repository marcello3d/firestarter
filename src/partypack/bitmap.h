#ifndef __bitmap_h__
#define __bitmap_h__

#include <allegro.h>
#include <vector>
#include <string>

#include "vector2d.h"
#include "region2d.h"
#include "resource.h"

class Bitmap : public Resource
{
protected:
	BITMAP *data;
	PALETTE pal;
public:
	enum BlitMode
	{
		Normal,
		Masked
	};
	
	Bitmap();
	Bitmap(BITMAP *b);
	~Bitmap();

	bool load(std::string fn);

	inline BITMAP *getABitmap() { return data; }
	inline RGB *getPalette() { return pal; }
	
	void drawBitmap(Bitmap *source, vector2i pos, BlitMode mode = Normal) { drawBitmap(source->getABitmap(), pos, mode); }
	void drawBitmap(Bitmap *source, vector2i pos, region2i reg, BlitMode mode = Normal) { drawBitmap(source->getABitmap(), pos, reg, mode); }
	void drawBitmap(BITMAP *source, vector2i pos, BlitMode mode = Normal);
	void drawBitmap(BITMAP *source, vector2i pos, region2i reg, BlitMode mode = Normal);
};

#endif
