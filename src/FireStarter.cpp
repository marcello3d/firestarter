#include "FireStarter.h"

#include "Global.h"
#include "GameState.h"
#include "MainGame.h"
#include "Music.h"
#include <loadpng.h>
//#include <algif.h>
//#include <jpgalleg.h>
#include "Data.h"
#include "Menu.h"
#include "partypack/bitmap.h"
#include "partypack/gamepad.h"
#include "partypack/buttons.h"

using namespace std;

FireStarter *gSys;

FireStarter::FireStarter() {
	gSys = this;
	game = 0;
	mp = 0;
	datafile = 0;
}
FireStarter::~FireStarter() {
	delete maingame;
	delete mp;
	delete input;
	if (datafile)
		unload_datafile(datafile);
}

void FireStarter::setupGame() {
	srand((unsigned)::time(NULL));
	install_mouse();
	//algif_init();
	//jpgalleg_init();
	register_png_file_type();

	smallscreen.create(200,300);

	datafile = load_datafile("data/fonts.dat");

	font     = (FONT *)datafile[DATA_FONTS_alpha].dat;
	scorefont= (FONT *)datafile[DATA_FONTS_score].dat;

	colors[0] = makecol(0,0,0);
	colors[1] = makecol(0x34,0x48,0x2B);//#34482B
	colors[2] = makecol(0x41,0x78,0x49);//#417849
	colors[3] = makecol(0x4C,0x9D,0x5C);//#4C9D5C
	colors[4] = makecol(0x5B,0xBD,0x63);//#5BBD63
	colors[5] = makecol(0x66,0xD5,0x68);

	input = new Gamepad(8);

	input->bind(UP,new KeyboardButton(KEY_UP));
	input->bind(DOWN,new KeyboardButton(KEY_DOWN));
	input->bind(LEFT,new KeyboardButton(KEY_LEFT));
	input->bind(RIGHT,new KeyboardButton(KEY_RIGHT));

	input->bind(A,new KeyboardButton(KEY_A));
	input->bind(B,new KeyboardButton(KEY_Z));

	input->bind(START,new KeyboardButton(KEY_ENTER));
	input->bind(SELECT,new KeyboardButton(KEY_RSHIFT));

	title = new Title;

	game = title;

	menu = new Menu;

	maingame = new MainGame;
	maingame->setup();


	mp = new MusicPlayer;
	music = rand()%2;

	switchMusic();

#ifdef DEBUGMODE
	game = maingame;
#endif
}
void FireStarter::switchMusic() {
	music = !music;
	if (music)
		mp->play(rm.load<Music>("data/lilbustr.xm"));
	else
		mp->play(rm.load<Music>("data/hybrid_rainbow.it"));

}
FireStarter::Title::Title() {
	title = gSys->rm.load<Bitmap>("data/title.png")->getABitmap();
}
void FireStarter::Title::update() {
	static bool press = false;
	if (gSys->input->isDown(A)||gSys->input->isDown(START)) {
		gSys->game = gSys->maingame;
		gSys->maingame->setup();
	}

}

void FireStarter::updateGameLogic() {
	poll_keyboard();
	poll_mouse();
	mp->update();

	if (key[KEY_ESC]) stop();

	static bool pressf9 = false;
	if (key[KEY_F9] && !pressf9) {
		toggleFullscreen();
		pressf9 = true;
	}
	if (!key[KEY_F9])
		pressf9 = false;

	if (key[KEY_F2])
		game = title;

	game->update();
}

void FireStarter::drawScreen(alBitmap &buffer) {

	game->draw(smallscreen);
	smallscreen.stretchBlitTo(buffer,0,0,400,600);
}

