#ifndef _MUSIC_PLAYER_H
#define _MUSIC_PLAYER_H

#include <aldumb.h>

class Music;

class MusicPlayer {

	public:
		MusicPlayer();
		~MusicPlayer();

		void play(Music *music);
		void update();
		void stop();

	private:
		AL_DUH_PLAYER *dp;

};

#endif

