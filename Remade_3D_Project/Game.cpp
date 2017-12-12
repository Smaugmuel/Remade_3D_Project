#include "Game.hpp"
#include "Window.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"
#include "DeferredScreenTarget.hpp"
#include "Input.hpp"
#include "PlayerCameraManager.hpp"
#include "Camera.hpp"

#include "SingleColorObject.hpp"
#include "TextureObject.hpp"

#include "SystemInformation.hpp"

/* Added with rastertek */
//#include "RenderTexture.hpp"
//#include "DebugWindow.hpp"
//#include "BitMap.hpp"

Game* Singleton<Game>::s_instance = nullptr;

Game::Game()
{
}

Game::~Game()
{
	Window::Delete();
	Direct3D::Delete();
	Input::Delete();
	PlayerCameraManager::Delete();
	ShaderManager::Delete();
	DeferredScreenTarget::Delete();
}

bool Game::Initialize()
{
	if (!Window::Get()->Initialize(Vector2i(WNDW, WNDH)))
	{
		return false;
	}
	if (!Direct3D::Get()->Initialize(Window::Get()))
	{
		return false;
	}
	if (!ShaderManager::Get()->Initialize(Direct3D::Get()->GetDevice()))
	{
		return false;
	}
	//if (!m_shaderManager->CreateShaderGroup(Direct3D::Get()->GetDevice(), "SingleColor"))
	//{
	//	return false;
	//}
	if (!DeferredScreenTarget::Get()->Initialize(Direct3D::Get()->GetDevice()))
	{
		return false;
	}


	// Cameras
	if (!PlayerCameraManager::Get()->Initialize())
	{
		return false;
	}

	Camera* cam = PlayerCameraManager::Get()->CreateCamera();
	if (!cam)
	{
		return false;
	}
	cam->SetDimensions(Window::Get()->GetDimensions());
	cam->SetPosition(Vector3f(0, 0, -20));
	cam->SetTarget(Vector3f(0, 0, 0));
	cam->Update();

	/*cam = PlayerCameraManager::Get()->CreateCamera();
	if (!cam)
	{
		return false;
	}
	cam->SetDimensions(Window::Get()->GetDimensions());
	cam->SetPosition(Vector3f(20, 0, 0));
	cam->SetTarget(Vector3f(0, 0, 0));
	cam->Update();

	cam = PlayerCameraManager::Get()->CreateCamera();
	if (!cam)
	{
		return false;
	}
	cam->SetDimensions(Window::Get()->GetDimensions());
	cam->SetPosition(Vector3f(0, 0, 20));
	cam->SetTarget(Vector3f(0, 0, 0));
	cam->Update();

	cam = PlayerCameraManager::Get()->CreateCamera();
	if (!cam)
	{
		return false;
	}
	cam->SetDimensions(Window::Get()->GetDimensions());
	cam->SetPosition(Vector3f(-20, 0, 0));
	cam->SetTarget(Vector3f(0, 0, 0));
	cam->Update();*/


	// Cubes
	for (unsigned int i = 0; i < 9; i++)
	{
		m_cubes.push_back(std::make_unique<TextureObject>());
		if (!m_cubes[i]->Initialize(Direct3D::Get()->GetDevice(), "cube_uv.obj"))
			return false;
		if (!m_cubes[i]->LoadTexture(Direct3D::Get()->GetDevice(), "../Textures/Torgue.png"))
			return false;
		m_cubes[i]->SetPosition(Vector3f(-5.0f + (i % 3) * 5.0f, 5.0f - (i / 3) * 5.0f, 1.0f));
	}

	// Floor
	m_floor = std::make_unique<SingleColorObject>();
	if (!m_floor->Initialize(Direct3D::Get()->GetDevice(), "cube.obj"))
		return false;
	m_floor->SetScale(100.0f, 0.02f, 100.0f);
	m_floor->SetPosition(0.0f, -10.0f, 0.0f);




	/* Added with rastertek */
	//m_bitMap = std::make_unique<BitMap>();
	//if (!m_bitMap->Initialize(Direct3D::Get()->GetDevice(), Window::Get()->GetDimensions(), Vector2i(100, 100)))
	//{
	//	return false;
	//}
	//m_renderTexture = std::make_unique<RenderTexture>();
	//if (!m_renderTexture->Initialize(Direct3D::Get()->GetDevice(), Window::Get()->GetDimensions()))
	//{
	//	return false;
	//}
	//m_debugWindow = std::make_unique<DebugWindow>();
	//if (!m_debugWindow->Initialize(Direct3D::Get()->GetDevice(), Window::Get()->GetDimensions(), Vector2i(100, 100)))
	//{
	//	return false;
	//}


	return true;
}

void Game::Run()
{
	MSG msg = { 0 };
	
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!ProcessInput())
			{
				break;
			}
			Update();
			Render();
		}
	}
}

bool Game::ProcessInput()
{
	Input::Get()->Update();

	if (Input::Get()->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Camera movement
	if (Input::Get()->IsKeyDown('A'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveRight(-0.01f);
	}
	if (Input::Get()->IsKeyDown('D'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveRight(0.01f);
	}
	if (Input::Get()->IsKeyDown('W'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveForward(0.01f);
	}
	if (Input::Get()->IsKeyDown('S'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveForward(-0.01f);
	}
	if (Input::Get()->IsKeyDown(VK_SPACE))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveUp(0.01f);
	}
	if (Input::Get()->IsKeyDown(VK_SHIFT))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveUp(-0.01f);
	}

	// Switch cameras
	if (Input::Get()->IsKeyPressed(VK_RIGHT))
	{
		PlayerCameraManager::Get()->ChangeUp();
	}
	if (Input::Get()->IsKeyPressed(VK_LEFT))
	{
		PlayerCameraManager::Get()->ChangeDown();
	}

	// Create camera
	if (Input::Get()->IsKeyPressed(VK_RETURN))
	{
		// Create a new camera as a copy of the current one
		PlayerCameraManager::Get()->CreateCamera();
	}

	// Look at origin
	if (Input::Get()->IsKeyDown('R'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->SetTarget(0.0f, 0.0f, 0.0f);
	}

	// Turn floor green
	if (Input::Get()->IsKeyPressed(VK_LBUTTON))
	{
		Vector2f mousePos = Input::Get()->MousePosition();
		m_floor->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Rotate cubes or camera
	if (Input::Get()->IsKeyDown(VK_RBUTTON))
	{
		Vector2f mouseMovement = Input::Get()->MouseMovement();

		for (unsigned int i = 0; i < m_cubes.size(); i++)
		{
			m_cubes[i]->Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
		}
	}
	else
	{
		Vector2f mouseMovement = Input::Get()->MouseMovement();

		PlayerCameraManager::Get()->GetCurrentCamera()->RotateUp(mouseMovement.y * 0.015f);
		PlayerCameraManager::Get()->GetCurrentCamera()->RotateRight(mouseMovement.x * 0.015f);
	}

	if (Input::Get()->IsKeyPressed('Z'))
	{
		static bool toggle = true;
		toggle = !toggle;

		if (toggle)
		{
			Direct3D::Get()->EnableZBuffer();
		}
		else
		{
			Direct3D::Get()->DisableZBuffer();
		}
	}

	return true;
}

void Game::Update()
{
	PlayerCameraManager::Get()->Update();

	for (unsigned int i = 0; i < m_cubes.size(); i++)
	{
		m_cubes[i]->Update();
	}
	m_floor->Update();
}

void Game::Render()
{
	// Clear color seems to not work correctly based on light position
	Direct3D::Get()->ClearDefaultTarget();

	if (false)
	{
		RenderDepth();
	}
	else if (false)
	{
		RenderDeferredFirstPass();
		//RenderShadowPass();

		RenderDeferredLightPass();
	}
	else if (true)
	{
		RenderNormal();
	}
	else
	{
		//RenderDeferredFirstPass();	// Render to the textures

		////Direct3D::Get()->DisableZBuffer();
		//m_bitMap->SetPosition(Vector2i(0, 0));
		//m_bitMap->SetShaderResourceView(Direct3D::Get()->GetDeferredShaderResourceViews()[1]);	// normals
		//m_bitMap->SetupVertexBuffer(Direct3D::Get()->GetDeviceContext());
		//m_bitMap->Render(Direct3D::Get()->GetDeviceContext());
		//
		////RenderToTexture();
		//

		//RenderNormal();
	}

	Direct3D::Get()->Present();
}

void Game::RenderNormal()
{
	ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::TEXTURE);
	ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), PlayerCameraManager::Get()->GetCamera(0), 1.0f);

	for (unsigned int i = 0; i < m_cubes.size(); i++)
	{
		ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_cubes[i].get());
		m_cubes[i]->Render(Direct3D::Get()->GetDeviceContext());
	}

	ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::SINGLE_COLOR);
	ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), PlayerCameraManager::Get()->GetCamera(0), 1.0f);
	ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_floor.get());
	m_floor->Render(Direct3D::Get()->GetDeviceContext());
}

//void Game::RenderToTexture()
//{
//	m_renderTexture->SetRenderTarget(Direct3D::Get()->GetDeviceContext(), Direct3D::Get()->GetDepthStencilView());
//	m_renderTexture->ClearTarget(Direct3D::Get()->GetDeviceContext(), Direct3D::Get()->GetDepthStencilView());
//
//	RenderNormal();
//
//	Direct3D::Get()->SetDefaultTarget();
//}

void Game::RenderDeferredFirstPass()
{
	Direct3D::Get()->SetDeferredTargets();
	Direct3D::Get()->ClearDeferredTargets();

	ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::D_SINGLE_COLOR);
	ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), PlayerCameraManager::Get()->GetCamera(0), 1.0f);

	for (unsigned int i = 0; i < m_cubes.size(); i++)
	{
		ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_cubes[i].get());
		m_cubes[i]->Render(Direct3D::Get()->GetDeviceContext());
	}
	ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_floor.get());
	m_floor->Render(Direct3D::Get()->GetDeviceContext());
}

void Game::RenderDepth()
{
	ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::DEPTH);
	ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), PlayerCameraManager::Get()->GetCamera(0), 1.0f);

	for (unsigned int i = 0; i < m_cubes.size(); i++)
	{
		ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_cubes[i].get());
		m_cubes[i]->Render(Direct3D::Get()->GetDeviceContext());
	}
	ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_floor.get());
	m_floor->Render(Direct3D::Get()->GetDeviceContext());
}

void Game::RenderShadowPass()
{
	Direct3D::Get()->SetShadowTarget();
	Direct3D::Get()->ClearShadowTarget();

	ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::D_SHADOW);
	ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), PlayerCameraManager::Get()->GetCamera(0), 1.0f);
	
	for (unsigned int i = 0; i < m_cubes.size(); i++)
	{
		ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_cubes[i].get());
		m_cubes[i]->Render(Direct3D::Get()->GetDeviceContext());
	}
	ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_floor.get());
	m_floor->Render(Direct3D::Get()->GetDeviceContext());
}

void Game::RenderDeferredLightPass()
{
	Direct3D::Get()->SetDefaultTarget();

	ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::D_LIGHT);

	ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), PlayerCameraManager::Get()->GetCamera(0), 1.0f);
	DeferredScreenTarget::Get()->Render(Direct3D::Get()->GetDeviceContext());
}