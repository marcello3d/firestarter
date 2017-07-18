#include "sprite.h"
#include "sprite_animator.h"

SpriteAnimator::SpriteAnimator(Sprite *spr) : sprite(spr), cur_seq(0), cur_timer(0), looping(true), mirror(false), playing(true)
{
}

void SpriteAnimator::draw(BITMAP *target, vector2i pos)
{
	if (!sprite) return;

	unsigned int fpos = (cur_timer * sprite->getSequenceFPS(cur_seq))/ANIMATOR_FPRS;
	unsigned int finalframe = MID(fpos, 0, sprite->getSequenceFrames(cur_seq));
	//region2i frame = sprite->getFrame(cur_seq, finalframe);



	//masked_blit(sprite->getFrameData(), target, frame.x, frame.y, pos.x, pos.y, frame.w, frame.h);
	if (mirror)
	{
		vector2i hotspot = sprite->getHotspot(cur_seq, finalframe);
		pos -= vector2i((sprite->getFrame(cur_seq, finalframe)).w - hotspot.x, hotspot.y);
		draw_sprite_h_flip(target, getBITMAP(), pos.x, pos.y);
	}
	else
	{
		pos -= sprite->getHotspot(cur_seq, finalframe);
		draw_sprite(target, getBITMAP(), pos.x, pos.y);
	}

	//region2i bbox = getBoundingBox();
	//rect(target, pos.x + bbox.x, pos.y + bbox.y, pos.x + bbox.x + bbox.w, pos.y + bbox.y + bbox.h, makecol(255, 0, 0));
}

BITMAP *SpriteAnimator::getBITMAP() {
	unsigned int fpos = (cur_timer * sprite->getSequenceFPS(cur_seq))/ANIMATOR_FPRS;
	unsigned int finalframe = MID(fpos, 0, sprite->getSequenceFrames(cur_seq));
	return sprite->getFrameBitmap(cur_seq, finalframe);
}

void SpriteAnimator::update()
{
	if (!sprite) return;

	if (!playing) return;

	cur_timer++;
	if (cur_timer * sprite->getSequenceFPS(cur_seq) >= sprite->getSequenceFrames(cur_seq) * ANIMATOR_FPRS)
	{
		if (looping)
			cur_timer = 0; //((cur_timer * sprite->getSequenceFPS(cur_seq))/ANIMATOR_FPRS) % sprite->getSequenceFrames(cur_seq);
		else
			cur_timer--;
	}
}
bool SpriteAnimator::isStopped() {
	return cur_timer * sprite->getSequenceFPS(cur_seq) >= sprite->getSequenceFrames(cur_seq) * ANIMATOR_FPRS;
}

void SpriteAnimator::play(unsigned int seq, bool loop)
{
	if (!sprite) return;
	if (seq >= sprite->getSequences()) return;

	looping = loop;
	playing = true;
	transition(seq, loop);
	cur_timer = 0;
}

void SpriteAnimator::transition(unsigned int seq, bool loop)
{
	if (!sprite) return;
	if (seq >= sprite->getSequences()) return;

	playing = true;
	cur_seq = seq;
	looping = loop;
}

region2i SpriteAnimator::getBoundingBox()
{
	unsigned int fpos = MID((cur_timer * sprite->getSequenceFPS(cur_seq))/ANIMATOR_FPRS, 0, sprite->getSequenceFrames(cur_seq));
	return sprite->getBound(cur_seq, fpos);
}

region2i SpriteAnimator::getAdjustedBoundingBox()
{
    unsigned int fpos = MID((cur_timer * sprite->getSequenceFPS(cur_seq))/ANIMATOR_FPRS, 0, sprite->getSequenceFrames(cur_seq));

    if (mirror)
    {
        vector2i hotspot = sprite->getHotspot(cur_seq, fpos);
        return sprite->getBound(cur_seq, fpos) + vector2i((sprite->getFrame(cur_seq, fpos)).w - getBoundingBox().w - 2 * getBoundingBox().x, 0) - vector2i((sprite->getFrame(cur_seq, fpos)).w - hotspot.x, hotspot.y);
    }
    else
        return sprite->getBound(cur_seq, fpos) - sprite->getHotspot(cur_seq, fpos);
}
