#include "AllegroBitmapGame.h"

AllegroBitmapGame::AllegroBitmapGame() {
}
AllegroBitmapGame::~AllegroBitmapGame() {
}

int AllegroBitmapGame::_initialize() {
	set_color_depth (getScreenBPP());
	
	if (!setFullscreen(false, true))
		return 1;
	
	if (!buffer.create(getScreenX(), getScreenY()))
		return 1;
	
	return 0;
}

bool AllegroBitmapGame::setFullscreen(bool _fullscreen, bool force) {
	if (fullscreen == _fullscreen && !force) 
		return false;
	
	fullscreen = _fullscreen;
	
	if (set_gfx_mode(fullscreen ? GFX_AUTODETECT : GFX_AUTODETECT_WINDOWED, getScreenX(), getScreenY(), 0, 0) != 0) 
		return false;
	
	set_display_switch_mode(fullscreen ? SWITCH_PAUSE : SWITCH_BACKGROUND);
	
	return true;
}
void AllegroBitmapGame::updateScreen() {
	acquire_screen();
	buffer.blitTo(screen, 0, 0);
	release_screen();
}

void AllegroBitmapGame::drawScreen() {
	buffer.acquire();
	drawScreen(buffer);
	buffer.release();
}

