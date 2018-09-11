#include "Engine.hpp"

#include "../GUI/FPS_CounterV2.hpp"

#include "../Events/EventDispatcher.hpp"

Engine::Engine()
{
}

Engine::~Engine()
{
}

bool Engine::Initialize(Vector2i windowSize)
{
	if (!m_frameWorkManager.Initialize(windowSize, MAX_NR_OF_LIGHTS))
		return false;
	if (!m_materialManager.Initialize(m_frameWorkManager.GetTextureManager()))
		return false;
	if (!m_modelManager.Initialize(m_frameWorkManager.GetVertexBufferManager(), &m_materialManager))
		return false;
	if (!m_sceneManager.Initialize(&m_modelManager, &m_materialManager, &m_frameWorkManager))
		return false;

	if (!m_fps.Initialize(m_frameWorkManager.GetGUIManager()))
		return false;

	m_isRunning = true;
	m_windowSize = windowSize;
	
	m_renderCameraIndex = -1;
	m_hasFirstPersonControls = false;

	m_t1 = Clock::now();
	m_t2 = Clock::now();

	return true;
}

bool Engine::Update()
{
	MSG msg = { 0 };

	/*
	Calculate frame time
	*/
	m_t1 = m_t2;
	m_t2 = Clock::now();
	long long nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(m_t2 - m_t1).count();
	float dt = static_cast<float>(nanoSeconds * 0.000000001);

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	/*
	Update input
	*/
	m_frameWorkManager.GetInput()->Update();
	
	if (m_frameWorkManager.GetInput()->IsKeyDown(VK_ESCAPE))
	{
		m_isRunning = false;
	}
	
	/*
	Update fps counter
	*/
	m_fps.Update(dt);


	if (m_renderCameraIndex == -1)
	{
		if (m_cameraManager.NrOfCameras() > 0)
		{
			// No camera is set, but a camera exists
			m_renderCameraIndex = 0;
		}
		else
		{
			// No camera is set, and no camera exists
		}
	}
	else
	{
		if (CameraV3* cam = m_cameraManager.GetCamera(m_renderCameraIndex); cam)
		{
			// A camera is set, and it exists
			if (m_hasFirstPersonControls)
				ControlCamera(dt);
			m_sceneManager.SetViewAndProjectionMatrices(cam->viewMatrix, cam->projectionMatrix);
		}
		else
		{
			// A camera is set, but it doesn't exist
			m_renderCameraIndex = -1;
		}
	}
	
	return m_isRunning;
}

void Engine::Clear(float r, float g, float b, float a)
{
	m_frameWorkManager.ClearGeometryPassRenderTargets(r, g, b, a);
	m_frameWorkManager.ClearLightPassRenderTargets(r, g, b, a);
	m_frameWorkManager.ClearFinalPassRenderTarget(r, g, b, a);
}

void Engine::Render()
{
	m_sceneManager.Render();
	m_frameWorkManager.GetGUIManager()->Render();
}

void Engine::Present()
{
	m_frameWorkManager.Present();
}

void Engine::EnableFirstPersonControls()
{
	m_hasFirstPersonControls = true;
}

void Engine::DisableFirstPersonControls()
{
	m_hasFirstPersonControls = false;
}

void Engine::ControlCamera(float dt)
{
	Input* input = m_frameWorkManager.GetInput();
	CameraV3* cam = m_cameraManager.GetCamera(m_renderCameraIndex);

	bool isChanged = false;

	float speed = 10.0f;
	if (input->IsKeyDown('B'))
		speed *= 10.0f;

	if (input->IsKeyDown('D'))
	{
		cam->MoveRight(speed * dt);
		isChanged = true;
	}
	if (input->IsKeyDown('A'))
	{
		cam->MoveRight(-speed * dt);
		isChanged = true;
	}
	if (input->IsKeyDown('W'))
	{
		cam->MoveForward(speed * dt);
		isChanged = true;
	}
	if (input->IsKeyDown('S'))
	{
		cam->MoveForward(-speed * dt);
		isChanged = true;
	}
	if (input->IsKeyDown(VK_SPACE))
	{
		cam->MoveUp(speed * dt);
		isChanged = true;
	}
	if (input->IsKeyDown(VK_SHIFT))
	{
		cam->MoveUp(-speed * dt);
		isChanged = true;
	}

	if (isChanged)
	{
		cam->UpdateViewMatrix();
	}
}

void Engine::ShowFPSCounter()
{
	m_fps.Show();
}

void Engine::HideFPSCounter()
{
	m_fps.Hide();
}

//void Engine::EnableDepthTests()
//{
//	m_frameWorkManager.GetDirect3D()->EnableZBuffer();
//	//Direct3D::Get()->EnableZBuffer();
//}
//
//void Engine::DisableDepthTests()
//{
//	m_frameWorkManager.GetDirect3D()->DisableZBuffer();
//	//Direct3D::Get()->DisableZBuffer();
//}

GUIManager * Engine::GetGUIManager()
{
	return m_frameWorkManager.GetGUIManager();
}

SceneManagerV3 * Engine::GetSceneManager()
{
	return &m_sceneManager;
}

CameraManagerV2 * Engine::GetCameraManager()
{
	return &m_cameraManager;
}

ModelManager * Engine::GetModelManager()
{
	return &m_modelManager;
}

MaterialManager * Engine::GetMaterialManager()
{
	return &m_materialManager;
}

Input * Engine::GetInput()
{
	return m_frameWorkManager.GetInput();
}

Vector2i Engine::GetWindowSize() const
{
	return m_windowSize;
}