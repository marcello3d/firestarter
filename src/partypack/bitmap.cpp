#include <allegro.h>
#include "bitmap.h"

using namespace std;

Bitmap::Bitmap() : data(NULL)
{
}

Bitmap::Bitmap(BITMAP *b) : data(b)
{
}

Bitmap::~Bitmap()
{
	if ((data) && (data != screen))
		destroy_bitmap(data);
}

bool Bitmap::load(string fn)
{
	return (data = load_bitmap(fn.c_str(), pal));
}

void Bitmap::drawBitmap(BITMAP *source, vector2i pos, BlitMode mode)
{
	ASSERT(source);
	
	if (mode == Normal)
		blit(source, data, 0, 0, pos.x, pos.y, source->w, source->h);
	else if (mode == Masked)
		masked_blit(source, data, 0, 0, pos.x, pos.y, source->w, source->h);
}
void Bitmap::drawBitmap(BITMAP *source, vector2i pos, region2i reg, BlitMode mode)
{
	ASSERT(source);
	
	if (mode == Normal)
		blit(source, data, reg.x, reg.y, pos.x, pos.y, reg.w, reg.h);
	else if (mode == Masked)
		masked_blit(source, data, reg.x, reg.y, pos.x, pos.y, reg.w, reg.h);
}
