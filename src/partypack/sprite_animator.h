#ifndef __sprite_animationor_h__
#define __sprite_animationor_h__

#include "sprite.h"

#define ANIMATOR_FPRS 100

class SpriteAnimator
{
private:
	Sprite *sprite;

	int cur_seq;	// Current sequence
	int cur_timer;	// Current timer
	bool looping;
	bool mirror;
	bool playing;
public:
	SpriteAnimator(Sprite *spr);

	void draw(BITMAP *target, vector2i pos);
	void update();

	void play(unsigned int seq, bool loop = false);
	void transition(unsigned int seq, bool loop = false);

	void stop() { playing = false; }
	bool isStopped();

	region2i getBoundingBox();
	region2i getAdjustedBoundingBox();

	inline int getCurrentSequence() { return cur_seq; }
	void setMirror(bool m) { mirror = m; }

	BITMAP *getBITMAP();

	void setLooping(bool _l) { looping = _l; }
};

#endif
