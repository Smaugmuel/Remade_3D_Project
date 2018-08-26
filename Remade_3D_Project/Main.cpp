#include "Game.hpp"
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game;

	if (game.Initialize())
	{
		game.Run();
	}

	return 0;
}