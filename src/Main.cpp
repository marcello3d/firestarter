#include "Main.h"
#include "FireStarter.h"

Log gLog;

int main() {
	gLog.start("log.txt", Log::vDebug);

	FireStarter *game = new FireStarter;
	game->run();
	delete game;

	allegro_exit();
	return 0;
} END_OF_MAIN();
