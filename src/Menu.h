
#ifndef _MENU_H
#define _MENU_H


#include "GameState.h"


class Menu : public GameState {
	public:
		Menu();
		~Menu();

		void setup();
		void update();
		void draw(alBitmap &bitmap);

	private:
		int loadstep;
		int levelCount;
		bool loading;
};



#endif
