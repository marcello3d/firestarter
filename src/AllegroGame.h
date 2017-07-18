
#ifndef _ALLEGRO_GAME_H
#define _ALLEGRO_GAME_H

class AllegroGame {
	public:
		AllegroGame();
		virtual ~AllegroGame();
		
		virtual int initialize();
		virtual void run();
		
		virtual void stop();
		
		virtual void updateScreen() = 0;
		
		int getFPS();
		int getCPS();
		
	protected:
		virtual int _initialize() = 0;
		
		virtual void setupGame() = 0;
		virtual void updateGameLogic() = 0;
		virtual void drawScreen() = 0;
		
		virtual char *getTitle() = 0;
		
		virtual int getScreenX() { return 640; }
		virtual int getScreenY() { return 480; }
		virtual int getScreenBPP() { return 32; }
		
		virtual bool getStartFullscreen() { return false; }
		
		virtual bool setFullscreen(bool fs, bool force = false) { return false; }
		virtual bool getFullscreen() { return fullscreen; }
		virtual bool toggleFullscreen() { return setFullscreen(!getFullscreen()); }
		
		bool fullscreen;
		bool initialized;
		bool running;
		
		static AllegroGame *game;
		
	private:
		static volatile int frameCount, calcCount, fps, cps;

		static volatile int timeCount;
		static void incrementSpeedCounter();
		
		static void fpsProc();
		static void closeProc();

};

#endif
