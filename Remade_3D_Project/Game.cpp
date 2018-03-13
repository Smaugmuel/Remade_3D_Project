#include "Game.hpp"

#include "Window.hpp"
#include "Input.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"
#include "DeferredScreenTarget.hpp"

#include "WindowSettings.hpp"

#include "ModelStorage.hpp"
#include "TextureStorage.hpp"
#include "ShaderStorage.hpp"
#include "SamplerStorage.hpp"
#include "ConstantBufferStorage.hpp"
#include "SceneStorage.hpp"

#include "PlayerCameraManager.hpp"

#include "PointLightManager.hpp"

#include "Collision.hpp"

#include "PlayState.hpp"
#include "SceneEditorState.hpp"

#include "EventDispatcher.hpp"

//#include "QuadTree.hpp"

// Binary, octagonal, decimal, hexadecimal notations
// 0b1111000;
// 0170;
// 120;
// 0x78;


Game* Singleton<Game>::s_instance = nullptr;

Game::Game()
{
	EventDispatcher::Get()->Subscribe(EventType::POP_GAMESTATE, this);
}

Game::~Game()
{
	Window::Delete();
	Input::Delete();
	Direct3D::Delete();
	ShaderManager::Delete();
	DeferredScreenTarget::Delete();
	
	PlayerCameraManager::Delete();
	
	PointLightManager::Delete();

	ModelStorage::Delete();
	TextureStorage::Delete();
	ShaderStorage::Delete();
	SamplerStorage::Delete();
	ConstantBufferStorage::Delete();

	Collision::Delete();

	EventDispatcher::Delete();

	SceneStorage::Delete();
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

	/* ============================================= Storages ============================================= */
	if (!ModelStorage::Get()->LoadTextureModel(Direct3D::Get()->GetDevice(), "cube_uv.obj"))
		return false;
	if (!ModelStorage::Get()->LoadTextureModel(Direct3D::Get()->GetDevice(), "turret.obj"))
		return false;
	if (!ModelStorage::Get()->LoadSingleColorModel(Direct3D::Get()->GetDevice(), "cube.obj"))
		return false;

	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "Torgue.png"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "BrickWallRaw.jpg"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "turret_tex_v3.png"))
		return false;
	
	/*if (!SceneStorage::Get()->LoadScene("Scene1_10000_cubes"))
		return false;
	if (!SceneStorage::Get()->LoadScene("Scene3_1_turret"))
		return false;
	if (!SceneStorage::Get()->LoadScene("Scene4_100_cubes"))
		return false;*/

	/* ============================================= Cameras ============================================= */
	if (!PlayerCameraManager::Get()->Initialize())
		return false;

	m_gameStateMachine.Push<SceneEditorState>();
	if (!m_gameStateMachine.Peek()->Initialize())
	{
		return false;
	}

	m_pop_game_state_flag = false;

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
			t1 = t2;
			t2 = Clock::now();

			long long nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
			float deltaTime = static_cast<float>(nanoSeconds * 0.000000001);

			if (m_gameStateMachine.Empty())
				break;

			m_gameStateMachine.Peek()->ProcessInput();
			m_gameStateMachine.Peek()->Update(deltaTime);
			m_gameStateMachine.Peek()->Render();

			if (m_pop_game_state_flag)
			{
				m_gameStateMachine.Pop();
				m_pop_game_state_flag = false;
			}
		}
	}
}

void Game::ReceiveEvent(const Event & e)
{
	switch (e.type)
	{
	case EventType::POP_GAMESTATE:
		m_pop_game_state_flag = true;
	default:
		break;
	}
}