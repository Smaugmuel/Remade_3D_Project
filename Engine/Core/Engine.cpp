#include "Engine.hpp"

#include "../FrameWork/Window.hpp"
#include "../FrameWork/Direct3D.hpp"

#include "../Render/Shaders/ShaderManager.hpp"
#include "../Render/DeferredScreenTarget.hpp"
#include "../Render/SamplerStorage.hpp"
#include "../Render/Shaders/ShaderStorage.hpp"
#include "../Render/SceneManager.hpp"

#include "../Buffers/ConstantBufferStorage.hpp"

#include "../Camera/PlayerCameraManager.hpp"

#include "../Objects/Models/ModelStorage.hpp"
#include "../Objects/Models/ModelStorageV2.hpp"
#include "../Objects/Materials/MaterialStorage.hpp"
#include "../Objects/Materials/MaterialStorageV2.hpp"
#include "../Objects/Textures/TextureStorage.hpp"
#include "../Objects/Textures/TextureStorageV2.hpp"

#include "../GUI/FPS_CounterV2.hpp"

#include "../Input/Input.hpp"

#include "../Events/EventDispatcher.hpp"

Engine* Singleton<Engine>::s_instance = nullptr;

Engine::Engine()
{
}

Engine::~Engine()
{
	m_fps.get()->RemoveGUIText();

	Window::Delete();
	Direct3D::Delete();
	ShaderManager::Delete();
	DeferredScreenTarget::Delete();
	SamplerStorage::Delete();
	ConstantBufferStorage::Delete();

	SceneManager::Delete();

	//ModelStorage::Delete();
	ModelStorageV2::Delete();
	MaterialStorageV2::Delete();
	TextureStorageV2::Delete();
	ShaderStorage::Delete();

	PlayerCameraManager::Delete();

	Input::Delete();
}

bool Engine::Initialize(int windowSizeX, int windowSizeY)
{
	return Engine::Initialize(Vector2i(windowSizeX, windowSizeY));
}

bool Engine::Initialize(Vector2i windowSize)
{
	if (!Window::Get()->Initialize(windowSize))
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

	if (!PlayerCameraManager::Get()->Initialize())
		return false;

	m_isRunning = true;

	m_windowSize = windowSize;

	m_fps = std::make_unique<FPSCounterV2>();
	m_fpsID = m_fps.get()->CreateGUIText();

	m_t1 = Clock::now();
	m_t2 = Clock::now();

	/*Vector2f v(10, 20);
	int buffer1 = m_frameWork.CreateVertexBuffer(sizeof(Vector2f), static_cast<void*>(&v));*/


	return true;
}

bool Engine::Update()
{
	MSG msg = { 0 };

	// Calculate frame time
	m_t1 = m_t2;
	m_t2 = Clock::now();
	long long nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(m_t2 - m_t1).count();
	float dt = static_cast<float>(nanoSeconds * 0.000000001);

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	// Update input, cameras, scene and fps counter
	Input::Get()->Update();
	PlayerCameraManager::Get()->Update();
	SceneManager::Get()->Update(dt);
	m_fps.get()->Update(dt);


	if (Input::Get()->IsKeyDown(VK_ESCAPE))
	{
		m_isRunning = false;
	}


	return m_isRunning;
}

void Engine::Clear(float r, float g, float b, float a)
{
	Direct3D::Get()->ClearDefaultTarget(r, g, b, a);
}

void Engine::Present()
{
	Direct3D::Get()->Present();
}

void Engine::ShowFPSCounter()
{
	m_fps.get()->Show();
}

void Engine::HideFPSCounter()
{
	m_fps.get()->Hide();
}

void Engine::EnableDepthTests()
{
	Direct3D::Get()->EnableZBuffer();
}

void Engine::DisableDepthTests()
{
	Direct3D::Get()->DisableZBuffer();
}

Vector2i Engine::GetWindowSize() const
{
	return m_windowSize;
}