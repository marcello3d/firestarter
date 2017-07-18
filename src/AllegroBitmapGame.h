
#ifndef _ALLEGRO_BITMAP_GAME_H
#define _ALLEGRO_BITMAP_GAME_H

#include "AllegroGame.h"
#include "alBitmap.h"

class AllegroBitmapGame : public AllegroGame {
	public:
		AllegroBitmapGame();
		virtual ~AllegroBitmapGame();
		
		virtual bool setFullscreen(bool fs, bool force = false);
		
		virtual void drawScreen();
		virtual void drawScreen(alBitmap &buffer) = 0;
		
	protected:
		virtual int _initialize();
		virtual void updateScreen();
		
		virtual alBitmap &getBuffer() { return buffer; }
		
		alBitmap buffer;
};

#endif
