#include "AllegroGame.h"

#include <allegro.h>

AllegroGame::AllegroGame() {
	initialized = false;
	running = false;
	fullscreen = getStartFullscreen();
}

AllegroGame::~AllegroGame() {
	remove_int(incrementSpeedCounter);
}

int AllegroGame::initialize() {
	if (initialized) return 0;

	allegro_init();
	set_close_button_callback(&closeProc);
	set_window_title(getTitle());

	install_keyboard();

	int i = 0;
	if ((i = _initialize())) return i;

	game = this;

	LOCK_VARIABLE(speedCounter);
	LOCK_FUNCTION(incrementSpeedCounter);
	install_int_ex(incrementSpeedCounter, BPS_TO_TIMER(100));

	LOCK_VARIABLE(running);

	LOCK_VARIABLE(calcCount);
	LOCK_VARIABLE(cps);

	LOCK_VARIABLE(frameCount);
	LOCK_VARIABLE(fps);

	LOCK_FUNCTION(fpsProc);
	install_int(fpsProc, 1000);


#ifdef COLOR_TABLE_H
	init_color_table();
#endif

	setupGame();

	initialized = true;

	return 0;
}
void AllegroGame::run() {
	if (!initialized)
		initialize();
	if (!initialized || running)
		return;

	running = true;

	while (running) {
		while (timeCount > 0) {
			updateGameLogic();
			timeCount--;
			if (timeCount>=100) {
				timeCount = 0;
				break;
			}
		}
		drawScreen();
		updateScreen();
		frameCount++;
	}

}

void AllegroGame::stop() {
	running = false;
}

int AllegroGame::getFPS() {
	return fps;
}
int AllegroGame::getCPS() {
	return cps;
}

void AllegroGame::fpsProc() {
   fps = frameCount;
   frameCount = 0;

   cps = calcCount;
   calcCount = 0;
} END_OF_FUNCTION(AllegroGame::fpsProc);

void AllegroGame::closeProc() {
	game->stop();
} END_OF_FUNCTION(AllegroGame::closeProc);

void AllegroGame::incrementSpeedCounter() {
	timeCount++;
}
END_OF_FUNCTION(incrementSpeedCounter);


volatile int AllegroGame::frameCount = 0;
volatile int AllegroGame::calcCount = 0;
volatile int AllegroGame::fps = 0;
volatile int AllegroGame::cps = 0;

volatile int AllegroGame::timeCount = 0;

AllegroGame *AllegroGame::game =  0;
