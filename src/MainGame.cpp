
#include <cmath>

#include "Global.h"

#include <iostream>
#include "MainGame.h"
#include "Menu.h"
#include "alBitmap.h"
#include "Random.h"
#include "Audio.h"
#include "partypack/vector2d.h"
#include "partypack/sprite_animator.h"
#include "partypack/bitmap.h"
#include "partypack/gamepad.h"

using namespace std;

static void switch_in() {
	gSys->maingame->paused = false;
}
static void switch_out() {
	gSys->maingame->paused = true;
}

MainGame::MainGame() {
	set_display_switch_callback(SWITCH_IN, switch_in);
	set_display_switch_callback(SWITCH_OUT, switch_out);

	console = gSys->rm.load<Bitmap>("data/interface.png")->getABitmap();
	tiles = gSys->rm.load<Bitmap>("data/roads.png")->getABitmap();

	playerimg = gSys->rm.load<Bitmap>("data/player.png")->getABitmap();
	playerptr = gSys->rm.load<Bitmap>("data/playerptr.png")->getABitmap();

	police = gSys->rm.load<Bitmap>("data/police.png")->getABitmap();
	policeptr = gSys->rm.load<Bitmap>("data/policeptr.png")->getABitmap();

	firetruck = gSys->rm.load<Bitmap>("data/firetruck.png")->getABitmap();
	fireptr = gSys->rm.load<Bitmap>("data/firetruckptr.png")->getABitmap();

	mask = gSys->rm.load<Bitmap>("data/mask.png")->getABitmap();

	heart = gSys->rm.load<Bitmap>("data/heart.png")->getABitmap();
	credits = gSys->rm.load<Bitmap>("data/credits.png")->getABitmap();
	gameover = gSys->rm.load<Bitmap>("data/gameover.png")->getABitmap();

	buildings[0] = gSys->rm.load<Bitmap>("data/rubble.png")->getABitmap();
	buildings[1] = gSys->rm.load<Bitmap>("data/construction0.png")->getABitmap();
	buildings[2] = gSys->rm.load<Bitmap>("data/building0.png")->getABitmap();
	buildings[3] = gSys->rm.load<Bitmap>("data/building1.png")->getABitmap();
	buildings[4] = gSys->rm.load<Bitmap>("data/building2.png")->getABitmap();
	buildings[5] = gSys->rm.load<Bitmap>("data/building3.png")->getABitmap();

	firefx = gSys->rm.load<Audio>("data/fire.wav")->getSAMPLE();
	sirenfx = gSys->rm.load<Audio>("data/siren.wav")->getSAMPLE();

	for (int i=0; i<BUILDING_COUNT; i++) {
		buildings2[i] = buildings[i];
		int color = buildings2[i].getMaskColor();
		for (int y=0; y<buildings2[i].getHeight(); y++)
			for (int x=0; x<buildings2[i].getWidth(); x++)
				if ((x&1)^(y&1))
					buildings2[i].setPixel(x,y,color);
	}

	pausescreen.create(200,300);
	pausescreen.clear(pausescreen.getMaskColor());
	for (int y=0; y<pausescreen.getHeight(); y++)
		for (int x=0; x<pausescreen.getWidth(); x++)
			if ((x&1)^(y&1))
				pausescreen.setPixel(x,y,gSys->colors[4]);

	city = 0;

}
MainGame::~MainGame() {
	delete city;
}

float constrain(float x, float min, float max) {
	if (x<min) return min;
	if (x>max) return max;
	return x;
}
float random( float min, float max ) {
   return min + float(rand())/float(RAND_MAX) * (max-min);
}

void MainGame::setup() {
	srand((unsigned)::time(NULL));
	score = 0;
	time = 0;
	hearts = 3;
	delete city;
	city = new LevelMap(this,50,100);
	canPlay = true;
	paused = false;
	player.startpos.x = random(200,city->width*40 - 200);
	player.startpos.y = random(200,city->height*10 - 200);
	player.pos = player.startpos;
	player.game = this;
	player.updatePos();
	city->makeCenter(player.mx,player.my);

	for (vector<GameObject*>::iterator ii = sprites.begin(); ii != sprites.end(); ii++)
		if ((*ii) != &player)
			delete (*ii);

	sprites.clear();
	sprites.push_back(&player);
	for (int i=0; i<4; i++) {
		sprites.push_back(new Police(this));
		sprites.push_back(new FireTruck(this));
	}
}


void MainGame::update() {


	if (!paused && canPlay) {

		if (hearts<=0) {
			canPlay = false;
			time = 0;
		}

		city->update();

		for (vector<GameObject*>::iterator ii = sprites.begin(); ii != sprites.end(); ii++) {
			(*ii)->update();
			(*ii)->updatePos();
		}

		viewport.x += (constrain(player.pos.x-100, -5, city->width*40 - 195)-viewport.x)/10;
		viewport.y += (constrain(player.pos.y-150, -30, city->height*10 - 270)-viewport.y)/10;

		time++;

		if (time%1000==0) {
			sprites.push_back(new Police(this));
			sprites.push_back(new FireTruck(this));
		}
	}
	if (!canPlay)
		time++;
	if (canPlay) {
		static int pressp = 0;
		if (gSys->input->isDown(START)) {
			if (!pressp) paused = !paused;
			pressp = 1;
		} else
			pressp = 0;
	} else {
		if (gSys->input->isDown(START))
			gSys->game = gSys->title;
	}
	static int presss = 0;
	if (gSys->input->isDown(SELECT)) {
		if (!presss) gSys->switchMusic();
		presss = 1;
	} else
		presss = 0;



}

void MainGame::draw(alBitmap &buffer) {
	buffer.clear(gSys->colors[5]);

	if (canPlay) {

		city->draw(buffer);

		for (vector<GameObject*>::iterator ii = sprites.begin(); ii != sprites.end(); ii++)
			(*ii)->draw(buffer,true);

		if (paused) {
			buffer.draw(pausescreen);
			city->drawMap(buffer);
		}

		buffer.draw(console);

		for (int i=0; i<hearts; i++) {
			buffer.draw(heart, 24+17*i, 280);
		}

	} else {
		buffer.draw(gameover);
		int y = (time-100)/5;
		if (y>credits->h-190) y = credits->h-190;
		buffer.draw(credits,0,70,0,y,200,190);
	}

	buffer.textprintf(gSys->scorefont, 98,3, gSys->colors[0], -1, "%08i", score);
}

void MainGame::GameObject::updatePos() {
	int x = (int)pos.x;
	int y = (int)pos.y;
	mx = (x/40);
	my = (y/20)*2;
	int color = getpixel(game->mask, x%40, y%20);
	if (getr(color)>128) my++;
	if (getg(color)>128) my--;
	if (getb(color)>128) mx--;
}

MainGame::Player::Player() {
	sa = new SpriteAnimator(gSys->rm.load<Sprite>("data/player.nws"));
}
MainGame::Player::~Player() {
	delete sa;
}
void MainGame::Player::update() {
	int dx = 0;
	int dy = 0;
	if (gSys->input->isDown(LEFT))
		dx--;
	if (gSys->input->isDown(RIGHT))
		dx++;
	if (gSys->input->isDown(UP))
		dy--;
	if (gSys->input->isDown(DOWN))
		dy++;

	pos.x += dx * 0.5;
	updatePos();
	if (game->city->getCellType(mx,my)>=100)
		pos.x-=dx * 0.5;

	pos.y += dy * 0.25;
	updatePos();
	if (game->city->getCellType(mx,my)>=100)
		pos.y -= dy * 0.25;

	if (dx||dy) {
		if (dy&&dx)
			sa->transition(dy>0 ? 1 : 3,true);
		else if (dy)
			sa->transition(dy>0 ? 0 : 4,true);
		else
			sa->transition(2,true);
		if (dx)
			sa->setMirror(dx>0);
		px = dx*4;
		py = dy*4;
	} else
		sa->stop();
	sa->update();

	if (gSys->input->isDown(A)) {
		pos.x += px;
		pos.y += py;
		updatePos();
		game->city->burnCell(mx,my,10);
		pos.x -= px;
		pos.y -= py;
		updatePos();
	}
}
void MainGame::Player::draw(alBitmap &buffer, bool overlay) {
	vector2f p(pos.x-game->viewport.x,pos.y-game->viewport.y);
	if (!overlay) {
		sa->draw(buffer.getBITMAP(),p);
	} else {
		buffer.draw(game->playerptr,(int)p.x-3,(int)p.y-20);
	}
}


MainGame::Police::Police(MainGame *_game) {
	game = _game;

	speed = 0;
	pos.x = random(200,game->city->width*40 - 200)/40*40;
	pos.y = random(200,game->city->height*10 - 200)/20*20 - 5;

	int ddx = (int)(game->player.pos.x-pos.x);
	int ddy = (int)(game->player.pos.y-pos.y);
	if (sqrt(ddx*ddx+ddy*ddy)<100) {
		if (game->player.pos.x<250)
			pos.x+=300;
		else
			pos.x-=300;
	}


	updatePos();
	while (game->city->getCellType(mx,my)!=1) {
		pos.y+=20;
		updatePos();
	}

	bool alt = (my%2) == 1;
	dx = 0;
	dy = 0;
	// top left
	if (game->city->getCellType(mx-(alt?0:1),my-1)==1) {
		dx--;dy--;
	}
	// top right
	else if (game->city->getCellType(mx+(alt?1:0),my-1)==1) {
		dx++;dy--;
	}
	// bottom left
	else if (game->city->getCellType(mx-(alt?0:1),my+1)==1) {
		dx--;dy++;
	}
	// bottom right
	else if (game->city->getCellType(mx+(alt?1:0),my+1)==1) {
		dx++;dy++;
	}
}
float sq(float a) {
	return a*a;
}
void MainGame::Police::update() {

	drive();

	updatePos();
	int ddx = (int)(game->player.pos.x-pos.x);
	int ddy = (int)(game->player.pos.y-pos.y);
	if (game->city->getCellType(game->player.mx,game->player.my)==1) {
		if (sqrt(ddx*ddx+ddy*ddy)<15) {
			game->player.pos = game->player.startpos;
			game->hearts--;
			play_sample(game->sirenfx, 200, 128, 1000, 0);
		} else if (sqrt(ddx*ddx+ddy*ddy)<100 && abs(ddx)>10 && abs(ddy)>10) {
			if (ddx!=0)
				ddx/=abs(ddx);
			if (ddy!=0)
				ddy/=abs(ddy);

			pos.x += ddx * 10;
			pos.y += ddy * 5;
			updatePos();
			pos.x -= ddx * 10;
			pos.y -= ddy * 5;
			if (game->city->getCellType(mx,my)==1) {
				dx = ddx;
				dy = ddy;
			}
		}

	}
}
void MainGame::Police::drive() {

	pos.x += dx * 0.5 * speed;
	pos.y += dy * 0.25 * speed;

	if (speed<1)
		speed+=0.01f;

	pos.x += dx * 4;
	pos.y += dy * 2;
	updatePos();
	pos.x -= dx * 4;
	pos.y -= dy * 2;
	if (game->city->getCellType(mx,my)!=1) {
		dx = -dx;
		dy = -dy;
	}

	pos.x -= dx * 8;
	pos.y -= dy * 4;
	updatePos();
	pos.x += dx * 8;
	pos.y += dy * 4;

	if (mx!=lmx || my!=lmy) {

		bool alt = (my%2) == 1;
		bool tl = game->city->getCellType(mx-(alt?0:1),my-1)==1;
		bool tr = game->city->getCellType(mx+(alt?1:0),my-1)==1;
		bool bl = game->city->getCellType(mx-(alt?0:1),my+1)==1;
		bool br = game->city->getCellType(mx+(alt?1:0),my+1)==1;
		if (dx==dy) {
			// if sides are open, 50% of turning
			if ((tr||bl)&&(((rand()%2)==0)||!br||!tl)) {
				// if both ways are open, 50% eliminate one
				if (tr&&bl&&(rand()%2)==0) bl = false;
				if (bl) {
					dx=-1; dy=1;
				} else {
					dx=1;  dy=-1;
				}
				speed = 0.5;
			}
		} else {
			// if sides are open, 50% of turning
			if ((tl||br)&&(((rand()%2)==0)||!bl||!tr)) {
				// if both ways are open, 50% eliminate one
				if (tl&&br&&(rand()%2)==0) br = false;
				if (br) {
					dx=1;  dy=1;
				} else {
					dx=-1; dy=-1;
				}
				speed = 0.5;
			}
		}
		lmx = mx;
		lmy = my;
	}
}
void MainGame::Police::draw(alBitmap &buffer, bool overlay) {
	vector2i p((int)pos.x-(int)game->viewport.x,(int)pos.y-(int)game->viewport.y);
	if (!overlay) {
		buffer.draw(game->police,p.x-10,p.y-3,
					dx<0 ? 0 : 16, dy>0 ? 0 : 11, 16, 11);
	} else {
		buffer.draw(game->policeptr,p.x-3,p.y-20);
	}
}
MainGame::FireTruck::FireTruck(MainGame *_game) : Police(_game) {
}
void MainGame::FireTruck::update() {
	drive();

	updatePos();

	bool alt = (my%2) == 1;
	restore(mx-(alt?0:1),my-1);
	restore(mx-(alt?1:0),my-1);
	restore(mx-(alt?0:1),my+1);
	restore(mx-(alt?1:0),my+1);
}
void MainGame::FireTruck::restore(int x, int y) {
	Cell *c = game->city->getCell(x,y);
	if (!c) return;
	if (c->fire) {
		delete c->fire;
		c->fire = 0;
		speed = 0;
	}
	if (c->type==100) {
		c->life = c->targetlife = 0;
		c->update();
		speed = 0;
	}
}
void MainGame::FireTruck::draw(alBitmap &buffer, bool overlay) {
	vector2i p((int)pos.x-(int)game->viewport.x,(int)pos.y-(int)game->viewport.y);
	if (!overlay) {
		buffer.draw(game->firetruck,p.x-8,p.y-3,
					dx<0 ? 0 : 21, dy>0 ? 0 : 16, 21, 16);
	} else {
		buffer.draw(game->fireptr,p.x-3,p.y-20);
	}
}


MainGame::LevelMap::LevelMap(MainGame *_game, int _width, int _height) {
	game = _game;
	width = _width;
	height = _height;
	cells = new Cell[width*height];
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++) {
			setCellType(x,y,(rand()%100)<30 ? 0 : 100+rand()%BUILDING_COUNT);
			Cell *c = getCell(x,y);
			c->x = x;
			c->y = y;
			c->game = game;
		}



	// Draw down right roads
	for (int startx=-width; startx<width; startx++) {

		int x = startx;
		int y = 0;
		while (x<width && y<height) {
			setCellType(x,y,1);
			if ((y%2)==1)
				x++;
			y++;
		}
		startx += rand()%5 + 1;
	}

	// draw down left roads
	for (int startx=rand()%5 ; startx<width*2; startx++) {
		int x = startx;
		int y = 0;
		bool makeroad = true;
		while (x<width*2 && y<height) {
			if (getCellType(x,y)==1)
				makeroad = (rand()%100)<75;
			if (makeroad)
				setCellType(x,y,1);
			if ((y%2)==0)
				x--;
			y++;
		}
		startx += rand()%5 + 1;
	}

}
void MainGame::LevelMap::makeCenter(int cx, int cy) {
	setCellType(cx,cy,-1);
	setCellType(cx,cy+3,-1);
	setCellType(cx,cy-2,1);
	setCellType(cx,cy+2,1);
	setCellType(cx-1,cy,1);
	setCellType(cx+1,cy,1);


	int xx = cy%2;
	setCellType(cx+xx,cy-1,1);
	setCellType(cx+xx,cy+1,1);
	setCellType(cx+xx-1,cy-1,1);
	setCellType(cx+xx-1,cy+1,1);
	/*
	for (int y=-4; y<5; y++) {
		for (int x=1; x<5-abs(y); x++) {
			setCellType(cx+x,cy+y,1);
			setCellType(cx-x,cy+y,1);
		}
		setCellType(cx+5-abs(y),cy+y,1);
		setCellType(cx-5+abs(y),cy+y,1);
	}*/
}

MainGame::LevelMap::~LevelMap() {
	delete [] cells;
}
void MainGame::LevelMap::update() {
	for (int y=0; y<height; y++)
		for (int x=0; x<width; x++)
			getCell(x,y)->update();
}
void MainGame::LevelMap::draw(alBitmap &buffer) {
	bool alt = false;
	for (int y=0; y<height; y++) {
		int yy = y*10 - (int)game->viewport.y;

		// draw roads/land
		for (int x=0; x<width; x++) {
			int xx = x*40 + (alt?20:0) - (int)game->viewport.x;

			Cell *cell = getCell(x,y);
			int type = cell->type;
			int roadtype = 0;
			if (type==1) {
				/*  following code to merge roads automatically using binary
						// tl, tr, bl, br
				*/
				// top left
				if (getCellType(x-(alt?0:1),y-1)==1)
					roadtype |= 8;
				// top right
				if (getCellType(x+(alt?1:0),y-1)==1)
					roadtype |= 4;
				// bottom left
				if (getCellType(x-(alt?0:1),y+1)==1)
					roadtype |= 2;
				// bottom right
				if (getCellType(x+(alt?1:0),y+1)==1)
					roadtype |= 1;
			}
			buffer.draw(game->tiles, xx, yy,
								 roadtype*38, 0, 40, 20);


		}
		alt = !alt;
	}
	alt = false;
	for (int y=0; y<height; y++) {
		int yy = y*10 - (int)game->viewport.y;

		// draw roads/land
		for (int x=0; x<width; x++) {
			int xx = x*40 + (alt?20:0) - (int)game->viewport.x;

			Cell *cell = getCell(x,y);
			int type = cell->type;
			if (type>=100) {
				BITMAP *img = game->buildings[type-100];
				buffer.draw(img, xx, yy - img->h + 20);
				if (cell->fire)
					cell->fire->draw(buffer.getBITMAP(),vector2i(xx,yy));
			}
		}

		// draw sprites

		for (vector<GameObject*>::iterator ii = game->sprites.begin(); ii != game->sprites.end(); ii++) {
			GameObject *s = *ii;
			if (s->my==y)
				s->draw(buffer);
		}

		// draw building overlays

		for (int x=0; x<width; x++) {
			int xx = x*40 + (alt?20:0) - (int)game->viewport.x;

			int type = getCellType(x,y);
			Cell *cell = getCell(x,y);
			if (type>=100) {
				buffer.draw(game->buildings2[type-100], xx, yy - game->buildings2[type-100].getHeight() + 20);
				if (cell->fire)
					cell->fire->draw(buffer.getBITMAP(),vector2i(xx,yy));
			}
		}
		alt = !alt;
	}
}

MainGame::Cell *MainGame::LevelMap::getCell(int x, int y) {
	if (x<0 || y<0 || x>=width || y>=height) return 0;
	return &cells[x+y*width];
}
void MainGame::Cell::burn(int _points) {
	if (type<=100 || fire!=0) return;
	fire = new SpriteAnimator(gSys->rm.load<Sprite>("data/fire.nws"));
	life = 0;
	if (abs(x-game->player.mx)<4 && abs(y-game->player.my)<8) {
		stop_sample(game->firefx);
		play_sample(game->firefx, 100, 128, 1000, 0);
	}
	points = _points;
}
void MainGame::Cell::update() {
	life++;
	if (fire) {
		int anim=0;
		if (life>10*1)
			anim++;
		if (life>20*1)
			anim++;
		if (life>30*1)
			anim++;
		if (life>40*1) {
			type = 100;
		}
		if (life>55*1)
			anim--;
		if (life>65*1)
			anim--;
		fire->transition(anim,true);
		fire->update();
		if (life>75*1) {
			delete fire;
			int xx = y%2;
			game->score += points;
			game->city->burnCell(x+xx,y-1,points+5);
			game->city->burnCell(x+xx,y+1,points+5);
			game->city->burnCell(x-1+xx,y-1,points+5);
			game->city->burnCell(x-1+xx,y+1,points+5);
			fire = 0;
			life = 0;
			targetlife = (rand()%8000)+4000;
		}
	} else if (type==101) {
		if (life>targetlife) {
			type = rand()%(BUILDING_COUNT-2)+102;
			life = 0;
		}
	} else if (type==100) {
		if (life>targetlife) {
			type=101;
			life = 0;
			targetlife = (rand()%6000)+6000;
		}
	}
}
void MainGame::LevelMap::drawMap(alBitmap &buffer) {
	bool alt = false;
	for (int y=0; y<height; y++) {
		int yy = y*1 - (int)(game->viewport.y+150)/20+150;

		// draw roads/land
		for (int x=0; x<width; x++) {
			int xx = x*2 + (alt?1:0) - (int)(game->viewport.x+100)/20+100;

			Cell *cell = getCell(x,y);
			int type = cell->type;
			int color = gSys->colors[4];
			if (type==1) color = gSys->colors[2];
			if (type==100) color = gSys->colors[1];
			if (type>100) color = gSys->colors[3];
			buffer.rectFill(xx,yy,xx+1,yy,color);
		}
		alt = !alt;
	}
	for (vector<GameObject*>::iterator ii = game->sprites.begin(); ii != game->sprites.end(); ii++) {
		GameObject *s = *ii;
		s->updatePos();
		int xx = s->mx*2 + (alt?1:0) - (int)(game->viewport.x+100)/20+100;
		int yy = s->my - (int)(game->viewport.y+150)/20+150;
		buffer.setPixel(xx,yy,gSys->colors[0]);
		if (s==&(game->player))
			buffer.draw(game->playerptr, xx-3,yy-7);
	}
}
