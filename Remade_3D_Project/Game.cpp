#include "Game.hpp"

//#include "../Engine/Render/RenderManager.hpp"

#include "../Engine/Objects/Models/ModelStorageV2.hpp"
#include "../Engine/Objects/Materials/MaterialStorageV2.hpp"
#include "../Engine/Objects/Textures/TextureStorageV2.hpp"

#include "../Engine/Lights/PointLightManager.hpp"
#include "../Engine/Math/Collision.hpp"
#include "../Engine/Events/EventDispatcher.hpp"

#include "PlayState.hpp"
#include "MainMenuState.hpp"
#include "SceneStorage.hpp"
//#include "SceneEditorState.hpp"

// These were added / changed when engine was reworked
#include "../Engine/GUI/GUIManager.hpp"
#include "../Engine/Render/SceneManager.hpp"
#include "../Engine/Core/Engine.hpp"
#include "../Engine/Camera/PlayerCameraManager.hpp"
#include "../Engine/Camera/Camera.hpp"
#include "../Engine/Input/Input.hpp"
#include <Windows.h>

Game* Singleton<Game>::s_instance = nullptr;

Game::Game()
{
	EventDispatcher::Get()->Subscribe(EventType::POP_GAMESTATE, this);
	EventDispatcher::Get()->Subscribe(EventType::FAILED_TO_INITIALIZE, this);
}

Game::~Game()
{
	EventDispatcher::Get()->Unsubscribe(EventType::POP_GAMESTATE, this);
	EventDispatcher::Get()->Unsubscribe(EventType::FAILED_TO_INITIALIZE, this);

	//RenderManager::Delete();
	
	SceneStorage::Delete();

	EventDispatcher::Delete();

	PointLightManager::Delete();
	Collision::Delete();

	// Added when engine was reworked
	Engine::Delete();
	GUIManager::Delete();
	SceneManager::Delete();
}

bool Game::Initialize()
{
	if (!Engine::Get()->Initialize(1500, 800))
		return false;
	
	/*if (!SceneStorage::Get()->LoadScene("Scene1"))
		return false;
	if (!SceneStorage::Get()->LoadScene("Scene3"))
		return false;
	if (!SceneStorage::Get()->LoadScene("Scene4"))
		return false;*/

#ifdef OLD_MODE
	//m_gameStateMachine.Push<SceneEditorState>();
	//m_gameStateMachine.Push<MainMenuState>();

	m_gameStateMachine.Push<PlayState>();
	if (!m_gameStateMachine.Peek()->Initialize())
		return false;

	m_pop_game_state_flag = false;
	m_shutdown_flag = false;
#endif

	Camera* cam = PlayerCameraManager::Get()->CreateCamera();
	cam->SetPosition(0, 0, -10);
	cam->SetTarget(0, 0, 0);
	cam->Update();

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			int obj1 = SceneManager::Get()->AddObject("SimpleCharacter.obj", Vector3f(-100 + 2 * i, -10, j * 2));
			if (obj1 == -1)
				return false;
		}
	}


	int gui1 = GUIManager::Get()->CreateText("hello world", Vector2i(0, 100), Fonts::COMIC_SANS_MS_16);
	if (gui1 == -1)
		return false;

	SceneManager::Get()->SetRenderMode(RenderModes::NORMAL);

	Engine::Get()->ShowFPSCounter();

	return true;
}

void Game::Run()
{
	Engine* engine = Engine::Get();
	SceneManager* sceneManager = SceneManager::Get();
	GUIManager* guiManager = GUIManager::Get();

	while (true)
	{
		/*if (m_pop_game_state_flag)
		{
			m_gameStateMachine.Pop();
			m_pop_game_state_flag = false;

			// Process initialization failures
			// (Could replace the Initialize() in most classes, since they won't need to return false)
			if (m_shutdown_flag)
			{
				while (!m_gameStateMachine.IsEmpty())
				{
					m_gameStateMachine.Pop();
					m_shutdown_flag = false;
				}
			}
		}*/

		if (Input::Get()->IsKeyPressed('T'))
		{
			sceneManager->SetObjectModel(5000, "generator.obj");
		}


		if (!engine->Update())
			break;

		engine->Clear(0, 1, 0, 1);
		sceneManager->Render();
		guiManager->Render();
		engine->Present();
	}
}

void Game::ReceiveEvent(const Event & e)
{
	switch (e.type)
	{
	case EventType::FAILED_TO_INITIALIZE:
		m_shutdown_flag = true;
	case EventType::POP_GAMESTATE:
		m_pop_game_state_flag = true;
		break;
	default:
		break;
	}
}