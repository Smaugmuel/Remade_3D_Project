#include "Game.hpp"
#include "Window.hpp"
#include "Direct3D.hpp"
#include "Camera.hpp"
#include "PlayerCameraManager.hpp"

#include "Object.hpp"

#include "ShaderManager.hpp"

#include "Input.hpp"

#include "DeferredScreenTarget.hpp"

Game* Singleton<Game>::s_instance = nullptr;



//	Deferred rendering steps
//		Shaders which receive object triangles and output a color- and normal texture
//		Shaders which receive the textures and output the final values
//		Some way to generalize the shaders
//			Issues with buffers may occur
//			A way to switch between which shaders to use
//		A fullscreen triangle on which to render
//
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
//		




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

	for (unsigned int i = 0; i < 9; i++)
	{
		m_objects.push_back(std::make_unique<Object>());
		m_objects[i]->Initialize(Direct3D::Get()->GetDevice(), "cube.obj");
		m_objects[i]->SetPosition(Vector3f(-5.0f + (i % 3) * 5.0f, 5.0f - (i / 3) * 5.0f, 1.0f));
	}

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

	if (!DeferredScreenTarget::Get()->Initialize(Direct3D::Get()->GetDevice()))
	{
		return false;
	}


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
			// Input ===========================================================================================
			Input::Get()->Update();
			if (Input::Get()->IsKeyDown(VK_ESCAPE))
			{
				break;
			}
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
			if (Input::Get()->IsKeyPressed(VK_RIGHT))
			{
				PlayerCameraManager::Get()->ChangeUp();
			}
			if (Input::Get()->IsKeyPressed(VK_LEFT))
			{
				PlayerCameraManager::Get()->ChangeDown();
			}
			if (Input::Get()->IsKeyDown('R'))
			{
				PlayerCameraManager::Get()->GetCurrentCamera()->SetTarget(0.0f, 0.0f, 0.0f);
			}
			if (Input::Get()->IsKeyPressed(VK_RBUTTON))
			{
				// Create a new camera as a copy of the current one
				PlayerCameraManager::Get()->CreateCamera();
			}
			if (Input::Get()->IsKeyDown(VK_LBUTTON))
			{
				Vector2f mouseMovement = Input::Get()->MouseMovement();

				for (unsigned int i = 0; i < m_objects.size(); i++)
				{
					m_objects[i]->Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
				}
			}
			else
			{
				Vector2f mouseMovement = Input::Get()->MouseMovement();

				PlayerCameraManager::Get()->GetCurrentCamera()->RotateUp(mouseMovement.y * 0.01f);
				PlayerCameraManager::Get()->GetCurrentCamera()->RotateRight(mouseMovement.x * 0.01f);
			}

			for (unsigned int i = 0; i < m_objects.size(); i++)
			{
				m_objects[i]->Update();
			}

			PlayerCameraManager::Get()->Update();


			// Rendering =======================================================================================

			// Clear color seems to not work, it's only black
			Direct3D::Get()->ClearDefaultTarget();
			//Direct3D::Get()->ClearAllTargets();

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


			// Render objects with first deferred pass
			Direct3D::Get()->SetDeferredTargets();
			Direct3D::Get()->ClearDeferredTargets();

			ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::D_SINGLE_COLOR);
			ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), Vector3f(0.0f, 0.0f, -10.0f), 1.0f);
			
			for (unsigned int i = 0; i < m_objects.size(); i++)
			{
				ShaderManager::Get()->SetPerObjectConstantBuffer(Direct3D::Get()->GetDeviceContext(), m_objects[i].get());
				m_objects[i]->Render(Direct3D::Get()->GetDeviceContext());
			}
			
			// Render screen quad with second deferred pass (light)
			Direct3D::Get()->SetDefaultTarget();
			
			ShaderManager::Get()->SetShaderType(Direct3D::Get()->GetDeviceContext(), ShaderType::D_LIGHT);
			ShaderManager::Get()->SetPerFrameConstantBuffer(Direct3D::Get()->GetDeviceContext(), PlayerCameraManager::Get()->GetCurrentCamera(), Vector3f(0.0f, 0.0f, -10.0f), 1.0f);	
			DeferredScreenTarget::Get()->Render(Direct3D::Get()->GetDeviceContext());


			Direct3D::Get()->Present();
		}
	}
}