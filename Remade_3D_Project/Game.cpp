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

#include "HUDObject.hpp"

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

	// Cubes
	for (unsigned int i = 0; i < 9; i++)
	{
		m_texturedCubes.push_back(std::make_unique<TextureObject>());
		if (!m_texturedCubes[i]->Initialize(Direct3D::Get()->GetDevice(), "cube_uv.obj"))
			return false;
		if (!m_texturedCubes[i]->LoadTexture(Direct3D::Get()->GetDevice(), "../Textures/Torgue.png"))
			return false;
		m_texturedCubes[i]->SetPosition(Vector3f(-5.0f + (i % 3) * 5.0f, 5.0f - (i / 3) * 5.0f, 1.0f));
	}

	// Floor
	m_coloredFloor = std::make_unique<SingleColorObject>();
	if (!m_coloredFloor->Initialize(Direct3D::Get()->GetDevice(), "cube.obj"))
		return false;
	m_coloredFloor->SetScale(100.0f, 0.02f, 100.0f);
	m_coloredFloor->SetPosition(0.0f, -10.0f, 0.0f);

	// HUD
	m_HUDObject = std::make_unique<HUDObject>();
	if (!m_HUDObject->Initialize(Direct3D::Get()->GetDevice(), "../Textures/Torgue.png", Vector2i(0, 0), Vector2i(400, 400)))
		return false;
	m_HUDObject->SetPosition(Vector2i(500, 500));
	m_HUDObject->SetDimensions(Vector2i(200, 200));


	m_renderMode = RenderMode::NORMAL_MODE;
	m_HUDMode = HUDMode::HUD_OFF;
	m_orthogonal = OrthogonalCamera::ORTHOGONAL_OFF;

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
	Input* input = Input::Get();

	input->Update();

	if (input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Camera movement
	if (input->IsKeyDown('A'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveRight(-0.01f);
	}
	if (input->IsKeyDown('D'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveRight(0.01f);
	}
	if (input->IsKeyDown('W'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveForward(0.01f);
	}
	if (input->IsKeyDown('S'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveForward(-0.01f);
	}
	if (input->IsKeyDown(VK_SPACE))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveUp(0.01f);
	}
	if (input->IsKeyDown(VK_SHIFT))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->MoveUp(-0.01f);
	}

	// Switch cameras
	if (input->IsKeyPressed(VK_RIGHT))
	{
		PlayerCameraManager::Get()->ChangeUp();
	}
	if (input->IsKeyPressed(VK_LEFT))
	{
		PlayerCameraManager::Get()->ChangeDown();
	}

	// Create camera
	if (input->IsKeyPressed(VK_RETURN))
	{
		// Create a new camera as a copy of the current one
		PlayerCameraManager::Get()->CreateCamera();
	}

	// Look at origin
	if (input->IsKeyDown('R'))
	{
		PlayerCameraManager::Get()->GetCurrentCamera()->SetTarget(0.0f, 0.0f, 0.0f);
	}

	// Turn floor green
	if (input->IsKeyPressed(VK_LBUTTON))
	{
		m_coloredFloor->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Toggle depth tests
	if (input->IsKeyPressed('Z'))
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

	// Toggle orthogonal projection
	if (input->IsKeyPressed('O'))
	{
		m_orthogonal = (OrthogonalCamera)(1 - m_orthogonal);
	}

	// Change render mode
	if (input->IsKeyPressed(49))	// 1
	{
		m_renderMode = (RenderMode)((m_renderMode + 1) % RenderMode::NR_OF_RENDER_MODES);
	}

	// Change hud mode
	if (input->IsKeyPressed(50))
	{
		m_HUDMode = (HUDMode)((m_HUDMode + 1) % HUDMode::NR_OF_HUD_MODES);

		switch (m_HUDMode)
		{
		case TORGUE_HUD:
			m_HUDObject->SetDefaultShaderResourceView();
			break;
		default:
			break;
		}
	}

	// Rotate cubes or camera
	if (input->IsKeyDown(VK_RBUTTON))
	{
		Vector2f mouseMovement = input->MouseMovement();

		for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
		{
			m_texturedCubes[i]->Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
		}
	}
	else
	{
		Vector2f mouseMovement = input->MouseMovement();

		PlayerCameraManager::Get()->GetCurrentCamera()->RotateUp(mouseMovement.y * 0.015f);
		PlayerCameraManager::Get()->GetCurrentCamera()->RotateRight(mouseMovement.x * 0.015f);
	}

	return true;
}

void Game::Update()
{
	PlayerCameraManager::Get()->Update();

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		m_texturedCubes[i]->Update();
	}
	m_coloredFloor->Update();
}

void Game::Render()
{
	Direct3D* d3d = Direct3D::Get();

	// Clear color seems to not work correctly based on light position
	d3d->ClearDefaultTarget();

	switch (m_renderMode)
	{
	case NORMAL_MODE:
		RenderNormal();
		break;
	case DEFERRED_MODE:
		RenderDeferredFirstPass();
		//RenderShadowPass();
		RenderDeferredLightPass();
		break;
	case DEPTH_MODE:
		RenderDepth();
		break;
	default:
		break;
	}

	switch (m_HUDMode)
	{
	case TORGUE_HUD:
		// The shader resource view is set when this mode is set
		RenderHUD();
		break;
	case DEFERRED_POSITIONS:
		m_HUDObject->SetShaderResourceView(d3d->GetDeferredShaderResourceViews()[0]);
		RenderHUD();
		break;
	case DEFERRED_NORMALS:
		m_HUDObject->SetShaderResourceView(d3d->GetDeferredShaderResourceViews()[1]);
		RenderHUD();
		break;
	case DEFERRED_COLORS:
		m_HUDObject->SetShaderResourceView(d3d->GetDeferredShaderResourceViews()[2]);
		RenderHUD();
		break;
	//case HUD_OFF:
		// Do not render HUD at all
	default:
		break;
	}

	d3d->Present();
}

void Game::RenderNormal()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);


	// Render textured objects
	shaders->SetShaderType(deviceContext, ShaderType::TEXTURE);

	shaders->SetPerFrameTextureConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix(),
		cam0->GetPosition(),
		1.0f);

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		shaders->SetPerObjectTextureConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix(), m_texturedCubes[i]->GetTexture());
		m_texturedCubes[i]->Render(deviceContext);
	}


	// Render single colored objects
	shaders->SetShaderType(deviceContext, ShaderType::SINGLE_COLOR);

	shaders->SetPerFrameSingleColorConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		cam->GetProjectionMatrix(),
		cam0->GetViewMatrix(),
		cam0->GetProjectionMatrix(),
		cam0->GetPosition(),
		1.0f);
	
	shaders->SetPerObjectSingleColorConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix(), m_coloredFloor->GetColor());
	
	m_coloredFloor->Render(deviceContext);
}
void Game::RenderDeferredFirstPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();

	d3d->SetDeferredTargets();
	d3d->ClearDeferredTargets();


	// Render textured objects
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);

	shaders->SetPerFrameDeferredTextureConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		shaders->SetPerObjectDeferredTextureConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix(), m_texturedCubes[i]->GetTexture());
		m_texturedCubes[i]->Render(deviceContext);
	}


	// Render single colored objects
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);

	shaders->SetPerFrameDeferredSingleColorConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		cam->GetProjectionMatrix());

	shaders->SetPerObjectDeferredSingleColorConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix(), m_coloredFloor->GetColor());

	m_coloredFloor->Render(deviceContext);
}
void Game::RenderDepth()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();

	shaders->SetShaderType(deviceContext, ShaderType::DEPTH);
	
	shaders->SetPerFrameDepthConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		shaders->SetPerObjectDepthConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		m_texturedCubes[i]->Render(deviceContext);
	}

	shaders->SetPerObjectDepthConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix());
	
	m_coloredFloor->Render(deviceContext);
}
void Game::RenderShadowPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCurrentCamera();

	d3d->SetShadowTarget();
	d3d->ClearShadowTarget();

	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	shaders->SetPerFrameDeferredShadowConstantBuffer(
		deviceContext,
		cam0->GetViewMatrix(),
		cam0->GetProjectionMatrix());
	
	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		shaders->SetPerObjectDeferredShadowConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		m_texturedCubes[i]->Render(deviceContext);
	}

	shaders->SetPerObjectDeferredShadowConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix());
	m_coloredFloor->Render(deviceContext);
}
void Game::RenderDeferredLightPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	d3d->SetDefaultTarget();

	shaders->SetShaderType(deviceContext, ShaderType::D_LIGHT);

	shaders->SetPerFrameDeferredLightConstantBuffer(
		deviceContext,
		cam0->GetViewMatrix(),
		cam0->GetProjectionMatrix(),
		m_HUDObject->GetShaderResourceView(),
		cam0->GetPosition(),
		1.0f);

	DeferredScreenTarget::Get()->Render(deviceContext);
}
void Game::RenderHUD()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();

	shaders->SetShaderType(deviceContext, ShaderType::HUD);

	shaders->SetPerObjectHUDConstantBuffer(
		deviceContext,
		m_HUDObject->GetShaderResourceView());

	// Disable depth test, so everything below is rendered above everything else
	d3d->DisableZBuffer();

	m_HUDObject->Render(deviceContext);

	// Reenable
	d3d->EnableZBuffer();
}
