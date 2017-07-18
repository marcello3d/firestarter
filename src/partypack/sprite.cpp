#include <allegro.h>
#include "sprite.h"

using namespace std;

Sprite::Sprite() : framedata(NULL)
{
}

Sprite::~Sprite()
{
}

bool Sprite::load(string fn)
{
	PACKFILE *fp;
	int q;

	fp = pack_fopen(fn.c_str(), "rb");
	if (!fp)
		return false;

	q = pack_igetw(fp);	// magic number - should be 0x1339
	if (q != 0x1339)
	{
		pack_fclose(fp);
		return false;
	}
	int version = pack_getc(fp);	// should be 2
	if (version != 2)
		return false;
	
	int flags = pack_igetl(fp);
	
	// read palette
	PALETTE pal;
	if (flags & 0x3 == 0x1)
	{
		for (int p = 0; p < 256; p++)
		{
			pal[p].r = pack_getc(fp);
			pal[p].g = pack_getc(fp);
			pal[p].b = pack_getc(fp);
		}
	}

	// construct frame data
	int w = pack_igetw(fp);
	int h = pack_igetw(fp);
	framedata = create_bitmap(w, h);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if (flags & 0x03 == 0x01)
			{
				int i = pack_getc(fp);
				putpixel(framedata, x, y, makecol((pal[i].r << 2) + (pal[i].r >> 4), (pal[i].g << 2) + (pal[i].g >> 4), (pal[i].b << 2) + (pal[i].b >> 4)));
			}
			else if (flags & 0x03 == 0x02)
			{
			}
			else if (flags & 0x03 == 0x03)
				putpixel(framedata, x, y, makecol(pack_getc(fp), pack_getc(fp), pack_getc(fp)));
		}
	}

	// load frame data
	// number of sequences
	int seqs = pack_igetw(fp);
	sequences.resize(seqs);
	
	// load them all
	for (int s = 0; s < seqs; ++s)
	{
		// FPS
		sequences[s].fps = pack_getc(fp);
		
		// number of frames
		int frames = pack_igetw(fp);
		sequences[s].frames.resize(frames);
		
		// load frames
		for (int f = 0; f < frames; ++f)
		{
			short x = pack_igetw(fp);
			short y = pack_igetw(fp);
			short w = pack_igetw(fp);
			short h = pack_igetw(fp);
			sequences[s].frames[f].region = region2i(x, y, w, h);
			sequences[s].frames[f].subRegion = create_sub_bitmap(framedata, x, y, w, h);
			
			x = pack_igetw(fp);
			y = pack_igetw(fp);
			w = pack_igetw(fp);
			h = pack_igetw(fp);
			sequences[s].frames[f].bound = region2i(x, y, w, h);
			
			x = pack_igetw(fp);
			y = pack_igetw(fp);
			sequences[s].frames[f].hotspot = vector2i(x, y);
		}
	}

	pack_fclose(fp);
	
	return true;
}
