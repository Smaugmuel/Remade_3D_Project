#include "Game.hpp"
#include <crtdbg.h>

/*

	-----------------------Initialization--------------------------
	Game
		Window
			HWND
		Direct3D (with Window)
			Device
			Device context
			Swap chain
			Back buffer
			Render target view
			Depth stencil buffer
			Depth stencil view
			Viewport
		ShaderManager (with Direct3D)
			DeferredColorShaderGroup
				VS_SingleColor
				PS_SingleColor
				PerFrame constant buffer
				PerObject constant buffer
		Object
			Model (via ModelFactory function)
				TextureModel or
		Camera



*/


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