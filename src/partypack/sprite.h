/*
* [ sprite.h ]
*/

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <allegro.h>
#include <vector>
#include <string>

#include "vector2d.h"
#include "region2d.h"
#include "resource.h"

class Sprite : public Resource
{
protected:
	class Sequence
	{
	public:
		class Frame
		{
		public:
			vector2i hotspot;
			region2i region;
			region2i bound;
		
			BITMAP *subRegion;
		};
		
		int fps;
		std::vector<Frame> frames;
	};
	
	std::vector<Sequence> sequences;
	
	BITMAP *framedata;
public:
	Sprite();
	~Sprite();

	bool load(std::string fn);

	inline BITMAP *getFrameData() { return framedata; }
	inline unsigned int getSequences() { return sequences.size(); }
	inline unsigned int getSequenceFrames(unsigned int s) { ASSERT(s < sequences.size()); return sequences[s].frames.size(); }
	inline unsigned int getSequenceFPS(unsigned int s) { ASSERT(s < sequences.size()); return sequences[s].fps; }
	
	inline region2i getFrame(unsigned int s, unsigned int f)
	{
		ASSERT(s < sequences.size() && f < sequences[s].frames.size());
		return sequences[s].frames[f].region;
	}
	inline region2i getBound(unsigned int s, unsigned int f)
	{
		ASSERT(s < sequences.size() && f < sequences[s].frames.size());
		return sequences[s].frames[f].bound;
	}
	inline vector2i getHotspot(unsigned int s, unsigned int f)
	{
		ASSERT(s < sequences.size() && f < sequences[s].frames.size());
		return sequences[s].frames[f].hotspot;
	}
	
	inline BITMAP *getFrameBitmap(unsigned int s, unsigned int f)
	{
		ASSERT(s < sequences.size() && f < sequences[s].frames.size());
		return sequences[s].frames[f].subRegion;
	}
};

#endif /* __SPRITE_H__ */
