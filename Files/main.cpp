#include "Game.h"
#include <ctime>
#include <cstdlib>
#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{ 
	Game game; 
	game.run(); 
	return 0; 
}