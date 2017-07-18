
#ifndef _MAIN_GAME_H
#define _MAIN_GAME_H


#include "partypack/vector2d.h"
#include <vector>
#include <list>
#include "GameState.h"

using namespace std;

class alBitmap;
class SpriteAnimator;

#define BUILDING_COUNT 6

class MainGame : public GameState {
	public:
		MainGame();
		~MainGame();

		void setup();
		void update();
		void draw(alBitmap &bitmap);

		int getScore() { return score; }
		bool paused;

	private:

		BITMAP *console, *tiles, *playerimg, *mask, *playerptr, *police, *policeptr, *firetruck, *fireptr, *heart, *gameover, *credits;
		BITMAP *buildings[BUILDING_COUNT];

		alBitmap buildings2[BUILDING_COUNT];

		alBitmap pausescreen;

		SAMPLE *firefx, *sirenfx;

		int score;

		int hearts;

		int time;

		bool canPlay;

		vector2f viewport;

		class GameObject {
			public:
				GameObject() {};
				virtual ~GameObject() {};
				virtual void update() = 0;

				virtual void draw(alBitmap &buffer, bool overlay = false) = 0;
				void updatePos();
			public:
				friend class MainGame;
				vector2f pos;
				int mx, my;
				MainGame *game;
		};

		vector<GameObject*> sprites;

		class Player : public GameObject {
			public:
				Player();
				virtual ~Player();
				virtual void update();

				virtual void draw(alBitmap &buffer, bool overlay = false);
			public:
				int px,py;
				vector2f startpos;
				SpriteAnimator *sa;
		};
		friend class Player;

		Player player;


		class Police : public GameObject {
			public:
				Police(MainGame *game);
				virtual ~Police() {};
				virtual void update();

				virtual void draw(alBitmap &buffer, bool overlay = false);
				void drive();
			protected:
				friend class MainGame;
				int dx,dy;
				int lmx,lmy;
				float speed;
		};
		class FireTruck : public Police {
			public:
				FireTruck(MainGame *game);
				virtual void update();
				virtual void draw(alBitmap &buffer, bool overlay = false);
				void restore(int x, int y);
		};

		struct Cell {
			Cell() {
				type = 0;
				life = 0;
				fire = 0;
			}
			void burn(int points);
			void update();

			MainGame *game;

			int type;
			int life;
			int targetlife;
			int points;
			int x,y;
			SpriteAnimator *fire;
		};

		class LevelMap {
			public:
				LevelMap(MainGame *game, int width, int height);
				~LevelMap();
				void update();
				void draw(alBitmap &buffer);
				void drawMap(alBitmap &buffer);

				int getCellType(int x, int y) {
					if (x<0 || y<0 || x>=width || y>=height) return 0;
					return getCell(x,y)->type;
				}
				Cell *getCell(int x, int y);
				void setCellType(int x, int y, int type) {
					if (x<0 || y<0 || x>=width || y>=height) return;
					cells[x+y*width].type = type;
				}
				void burnCell(int x, int y, int points) {
					if (x<0 || y<0 || x>=width || y>=height) return;
					getCell(x,y)->burn(points);
				}
				void makeCenter(int x, int y);
			private:
				friend class MainGame;
				friend class Police;
				MainGame *game;
				int width, height;
				MainGame::Cell *cells;
		};
		friend class LevelMap;

		LevelMap *city;
};



#endif
