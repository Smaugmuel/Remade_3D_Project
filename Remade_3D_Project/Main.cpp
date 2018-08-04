#include "Game.hpp"
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (Game::Get()->Initialize())
	{
		Game::Get()->Run();
	}

	Game::Delete();

	return 0;
}