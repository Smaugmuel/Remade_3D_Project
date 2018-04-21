#include "Game.hpp"

#include "Window.hpp"
#include "Input.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"
#include "DeferredScreenTarget.hpp"
#include "RenderManager.hpp"

#include "WindowSettings.hpp"

#include "ModelStorage.hpp"
#include "MaterialStorage.hpp"
#include "TextureStorage.hpp"
#include "ShaderStorage.hpp"
#include "SamplerStorage.hpp"
#include "ConstantBufferStorage.hpp"
#include "SceneStorage.hpp"

#include "PlayerCameraManager.hpp"

#include "PointLightManager.hpp"

#include "Collision.hpp"

#include "PlayState.hpp"
#include "MainMenuState.hpp"
//#include "SceneEditorState.hpp"

#include "EventDispatcher.hpp"


#include "World.hpp"

//#include "QuadTree.hpp"

// Binary, octagonal, decimal, hexadecimal notations
//int x = 0b1111000;
//int x = 0170;
//int x = 120;
//int x = 0x78;


Game* Singleton<Game>::s_instance = nullptr;

Game::Game()
{
	EventDispatcher::Get()->Subscribe(EventType::POP_GAMESTATE, this);
	EventDispatcher::Get()->Subscribe(EventType::FAILED_TO_INITIALIZE, this);
}

Game::~Game()
{
	EventDispatcher::Get()->Subscribe(EventType::POP_GAMESTATE, this);
	EventDispatcher::Get()->Subscribe(EventType::FAILED_TO_INITIALIZE, this);

	Window::Delete();
	Input::Delete();
	Direct3D::Delete();
	ShaderManager::Delete();
	DeferredScreenTarget::Delete();
	RenderManager::Delete();
	
	ModelStorage::Delete();
	MaterialStorage::Delete();
	TextureStorage::Delete();
	ShaderStorage::Delete();
	SamplerStorage::Delete();
	ConstantBufferStorage::Delete();
	SceneStorage::Delete();

	EventDispatcher::Delete();

	PlayerCameraManager::Delete();
	PointLightManager::Delete();
	Collision::Delete();


	World::Delete();
}

bool Game::Initialize()
{
	if (!Window::Get()->Initialize(Vector2i(WNDW, WNDH)))
		return false;
	if (!Direct3D::Get()->Initialize(Window::Get()))
		return false;
	if (!ShaderManager::Get()->Initialize(Direct3D::Get()->GetDevice()))
		return false;
	if (!DeferredScreenTarget::Get()->Initialize(Direct3D::Get()->GetDevice()))
		return false;
	if (!SamplerStorage::Get()->Initialize(Direct3D::Get()->GetDevice()))
		return false;
	if (!ConstantBufferStorage::Get()->Initialize(Direct3D::Get()->GetDevice()))
		return false;

	/* =========================================== Load models =========================================== */
	if (!ModelStorage::Get()->LoadTextureModel(Direct3D::Get()->GetDevice(), "cube_uv.obj"))
		return false;
	if (!ModelStorage::Get()->LoadTextureModel(Direct3D::Get()->GetDevice(), "turret.obj"))
		return false;
	if (!ModelStorage::Get()->LoadTextureModel(Direct3D::Get()->GetDevice(), "generator.obj"))
		return false;
	if (!ModelStorage::Get()->LoadSingleColorModel(Direct3D::Get()->GetDevice(), "cube.obj"))
		return false;
	if (!ModelStorage::Get()->CreateLineModel(Direct3D::Get()->GetDevice()))
		return false;

	/*if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "Torgue.png"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "BrickWallRaw.jpg"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "turret_tex_v3.png"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "Icons/SelectIcon.png"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "Icons/MoveIcon.png"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "Icons/PlaceIcon.png"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "Icons/SaveIcon.png"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "generator_tex.png"))
		return false;
	
	if (!SceneStorage::Get()->LoadScene("Scene1"))
		return false;
	if (!SceneStorage::Get()->LoadScene("Scene3"))
		return false;
	if (!SceneStorage::Get()->LoadScene("Scene4"))
		return false;*/

	/* ============================================= Cameras ============================================= */
	if (!PlayerCameraManager::Get()->Initialize())
		return false;

	/* ============================================= States ============================================== */
	//m_gameStateMachine.Push<PlayState>();
	//m_gameStateMachine.Push<SceneEditorState>();
	m_gameStateMachine.Push<MainMenuState>();
	if (!m_gameStateMachine.Peek()->Initialize())
	{
		return false;
	}

	m_pop_game_state_flag = false;
	m_shutdown_flag = false;

	return true;
}

void Game::Run()
{
	MSG msg = { 0 };
	
	Time t1 = Clock::now();
	Time t2 = Clock::now();

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/* ======================================== Update delta time ======================================== */
			t1 = t2;
			t2 = Clock::now();

			long long nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
			float deltaTime = static_cast<float>(nanoSeconds * 0.000000001);

			/* ========================================== Update input =========================================== */
			Input::Get()->Update();

			/* =========================== Exit game if there is no current game state =========================== */
			if (m_gameStateMachine.Empty())
				break;

			/* ======================================== Process game state ======================================= */
			m_gameStateMachine.Peek()->ProcessInput();
			m_gameStateMachine.Peek()->Update(deltaTime);
			m_gameStateMachine.Peek()->Render();


			/* =============================== Process requests to exit game states ============================== */
			if (m_pop_game_state_flag)
			{
				m_gameStateMachine.Pop();
				m_pop_game_state_flag = false;

				// Process initialization failures
				// (Could replace the Initialize() in most classes, since they won't need to return false)
				if (m_shutdown_flag)
				{
					while (!m_gameStateMachine.Empty())
					{
						m_gameStateMachine.Pop();
						m_shutdown_flag = false;
					}
				}
			}
		}
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