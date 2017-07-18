#ifndef _FireStarter_H
#define _FireStarter_H

#include "AllegroBitmapGame.h"
#include "partypack/resourcemanager.h"
#include "MusicPlayer.h"
#include "GameState.h"


class LevelSet;
class Menu;
class MainGame;
class Gamepad;

#define UP 		0
#define DOWN 	1
#define LEFT 	2
#define RIGHT 	3
#define A 		4
#define B 		5
#define START 	6
#define SELECT	7


class FireStarter : public AllegroBitmapGame {
	public:
		FireStarter();
		virtual ~FireStarter();

		virtual int getScreenX() { return 400; }
		virtual int getScreenY() { return 600; }
		virtual int getScreenBPP() { return 16; }

		virtual void setupGame();
		virtual void updateGameLogic();
		virtual void drawScreen(alBitmap &buffer);

		virtual char *getTitle() { return "Fire Starter"; }

		void switchMusic();
	public:

		//enum { UP, DOWN, LEFT, RIGHT, A, B, START, SELECT };

		alBitmap smallscreen;

		ResourceManager rm;
		MusicPlayer *mp;

		int music;

		DATAFILE *datafile;
		FONT *font, *scorefont;

		GameState *game;

		Menu *menu;
		MainGame *maingame;

		Gamepad *input;

		int colors[6];

		class Title : public GameState {
			public:
				Title();
				~Title() {}
				void draw(alBitmap &bitmap) { bitmap.draw(title,0,0); }
				void update();
			private:
				BITMAP *title;
		};
		Title *title;

};

#endif
