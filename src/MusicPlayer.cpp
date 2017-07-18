#include "MusicPlayer.h"

#include "Music.h"

MusicPlayer::MusicPlayer() {
	dumb_register_stdfiles();
	install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
	dp = 0;
}

MusicPlayer::~MusicPlayer() {
	stop();
	dumb_exit();
}

void MusicPlayer::update() {
	if (dp!=0)
		al_poll_duh(dp);
}

void MusicPlayer::play(Music *music) {
	stop();

	dp = al_start_duh(music->getDUH(), 2, 0, 1.0f, 4096, 48000);
}
void MusicPlayer::stop() {
	if (dp==0) return;
	al_stop_duh(dp);
	dp = 0;
}
