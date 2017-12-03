#include "Game.hpp"
#include "Window.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"
#include "DeferredScreenTarget.hpp"
#include "Input.hpp"
#include "PlayerCameraManager.hpp"
#include "Camera.hpp"
#include "Object.hpp"

Game* Singleton<Game>::s_instance = nullptr;

//	If generalizing a vertex shader, which parts will be different between them?
//		Constant buffers
//			Per frame
//			Per object
//		Input layout
//	How can these be solved?
//		Constant buffer using templates
//			When setting the values of the buffer, they need to be sent to the buffer directly, since the vertex shader can't deal with all kinds of parameters
//			Template buffer with variadic arguments might work
//			The shader simply passes the arguments along to the buffer
//			When mapping the buffer, variadic arguments can be received as well
//			Given an assignment operator, could the buffer update itself? Just use a SetVaules(args...) instead	


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
	if (!Window::Get()->Initialize(Vector2i(640, 640)))
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
	cam->SetPosition(Vector3f(0, 0, -10));
	cam->SetTarget(Vector3f(0, 0, 0));
	cam->Update();


	// Cubes
	for (unsigned int i = 0; i < 9; i++)
	{
		m_cubes.push_back(std::make_unique<Object>());
		if (!m_cubes[i]->Initialize(Direct3D::Get()->GetDevice(), "cube.obj"))
			return false;
		m_cubes[i]->SetPosition(Vector3f(-5.0f + (i % 3) * 5.0f, 5.0f - (i / 3) * 5.0f, 1.0f));
	}

	// Floor
	m_floor = std::make_unique<Object>();
	if (!m_floor->Initialize(Direct3D::Get()->GetDevice(), "cube.obj"))
		return false;
	m_floor->SetScale(100.0f, 0.02f, 100.0f);
	m_floor->SetPosition(0.0f, -10.0f, 0.0f);

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
	//ShaderManager::Get()->SetShaderGroup(Direct3D::Get()->GetDeviceContext(), ShaderType::SINGLE_COLOR);
	////m_shaderManager->SetShaderGroup(Direct3D::Get()->GetDeviceContext(), "SingleColor");
	//
	//ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera());
	//
	//Direct3D::Get()->SetDeferredTargets();		
	//
	//for (unsigned int i = 0; i < m_objects.size(); i++)
	//{
	//	ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_objects[i].get());
	//	m_objects[i]->Render(Direct3D::Get()->GetDeviceContext());
	//}
	//
	//Direct3D::Get()->SetDefaultTarget();


	// Clear color seems to not work, it's only black
	Direct3D::Get()->ClearAllTargets();

	//RenderDepth();

	RenderDeferredFirstPass();
	RenderDeferredLightPass();

	Direct3D::Get()->Present();
}

void Game::RenderDeferredFirstPass()
{
	Direct3D::Get()->SetDeferredTargets();

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
	//Direct3D::Get()->SetShadowTarget();
	//ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::SHADOW);
	//ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), PlayerCameraManager::Get()->GetCamera(0), 1.0f);
	//
	//for (unsigned int i = 0; i < m_cubes.size(); i++)
	//{
	//	ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_cubes[i].get());
	//	m_cubes[i]->Render(Direct3D::Get()->GetDeviceContext());
	//}
	//ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_floor.get());
	//m_floor->Render(Direct3D::Get()->GetDeviceContext());
}

void Game::RenderDeferredLightPass()
{
	Direct3D::Get()->SetDefaultTarget();

	ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::D_LIGHT);
	ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), PlayerCameraManager::Get()->GetCamera(0), 1.0f);
	DeferredScreenTarget::Get()->Render(Direct3D::Get()->GetDeviceContext());
}