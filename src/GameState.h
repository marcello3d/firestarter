#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include "alBitmap.h"

class GameState {
	public:
		virtual ~GameState() {};
		virtual void update() = 0;
		virtual void draw(alBitmap &bitmap) = 0;
};

#endif
