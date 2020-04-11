#include "Game.hpp"
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand(static_cast<unsigned int>(time(NULL)));

	Game game;

	if (game.Initialize())
	{
		game.Run();
	}

	return 0;
}