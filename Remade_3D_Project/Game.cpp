#include "Game.hpp"
#include "Window.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"
#include "DeferredScreenTarget.hpp"
#include "Input.hpp"
#include "PlayerCameraManager.hpp"
#include "Camera.hpp"

#include "HUDObject.hpp"

#include "PointLight.hpp"

#include "FPS_Counter.hpp"

#include "Character.hpp"

#include "ModelStorage.hpp"
#include "TextureStorage.hpp"

#include "SingleColorObject.hpp"
#include "TextureObject.hpp"

#include "SingleColorModel.hpp"
#include "TextureModel.hpp"

#include "ShaderStorage.hpp"

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
	ModelStorage::Delete();
	TextureStorage::Delete();
	ShaderStorage::Delete();
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
	cam->SetPosition(Vector3f(0, 10, -20));
	cam->SetTarget(Vector3f(0, 0, 0));
	cam->Update();



	if (!ModelStorage::Get()->LoadTextureModel(Direct3D::Get()->GetDevice(), "cube_uv.obj"))
	{
		return false;
	}
	if (!ModelStorage::Get()->LoadSingleColorModel(Direct3D::Get()->GetDevice(), "cube.obj"))
	{
		return false;
	}

	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "../Textures/Torgue.png"))
	{
		return false;
	}


	// Cubes
	unsigned int nrOfCubesX = 10;
	unsigned int nrOfCubesY = 10;
	unsigned int nrOfCubesZ = 10;
	float distance = 4;
	Vector3f startPos = Vector3f((nrOfCubesX - 1) * -0.5f, 1.0f, (nrOfCubesZ - 1) * -0.5f) * distance;
	unsigned int i = 0;

	for (unsigned int z = 0; z < nrOfCubesZ; z++)
	{
		for (unsigned int y = 0; y < nrOfCubesY; y++)
		{
			for (unsigned int x = 0; x < nrOfCubesX; x++)
			{
				m_texturedCubes.push_back(std::make_unique<TextureObject>());
				if (!m_texturedCubes[i]->Initialize())
					return false;

				m_texturedCubes[i]->SetModelName("cube_uv.obj");
				m_texturedCubes[i]->SetTextureName("../Textures/Torgue.png");

				m_texturedCubes[i]->SetPosition(startPos + Vector3f(x, y, z) * distance);
				i++;
			}
		}
	}

	// Floor
	m_coloredFloor = std::make_unique<SingleColorObject>();
	if (!m_coloredFloor->Initialize())
		return false;

	m_coloredFloor->SetModelName("cube.obj");

	m_coloredFloor->SetScale(100.0f, 0.02f, 100.0f);
	m_coloredFloor->SetPosition(0.0f, 0.0f, 0.0f);

	// HUD
	m_HUDObject = std::make_unique<HUDObject>();
	if (!m_HUDObject->Initialize(Direct3D::Get()->GetDevice(), "../Textures/Torgue.png", Vector2i(0, 0), Vector2i(400, 400)))
		return false;
	m_HUDObject->SetPosition(Vector2i(500, 500));
	m_HUDObject->SetDimensions(Vector2i(200, 200));

	
	for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
	{
		m_pointLights[i] = std::make_unique<PointLight>();
		if (!m_pointLights[i]->Initialize(Direct3D::Get()->GetDevice(), Vector2f(static_cast<float>(WNDW), static_cast<float>(WNDH)) * 4.0f))
			return false;
		m_pointLights[i]->SetPosition(Vector3f(-40.0f + 20.0f * i, 20.0f, -20.0f));
		m_pointLights[i]->SetTarget(Vector3f(0.0f, 0.0f, 0.0f));
		
		m_pointLights[i]->Update();
	}
	m_nrOfLights = 1;


	m_fpsCounter = std::make_unique<FPSCounter>();
	m_fpsCounter->Initialize(Direct3D::Get()->GetDevice(), Direct3D::Get()->GetDeviceContext());


	m_renderMode = RenderMode::NORMAL_MODE;
	m_HUDMode = HUDMode::HUD_OFF;
	m_orthogonal = OrthogonalMode::ORTHOGONAL_OFF;


	m_player = std::make_unique<Character>();
	m_player->SetMovementSpeed(20.0f);
	m_player->SetPosition(PlayerCameraManager::Get()->GetCurrentCamera()->GetPosition());
	m_player->SetLookDirection(PlayerCameraManager::Get()->GetCurrentCamera()->GetTargetDirection());

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
			if (!ProcessInput())
			{
				break;
			}
			t1 = t2;
			t2 = Clock::now();

			int nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
			double seconds = nanoSeconds * 0.000000001;

			Update(seconds);
			Render();
		}
	}
}

bool Game::ProcessInput()
{
	Input* input = Input::Get();
	PlayerCameraManager* manager = PlayerCameraManager::Get();

	input->Update();

	if (input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Character movement
	m_player->SetIsMovingRight(input->IsKeyDown('D'));
	m_player->SetIsMovingLeft(input->IsKeyDown('A'));
	m_player->SetIsMovingForward(input->IsKeyDown('W'));
	m_player->SetIsMovingBackward(input->IsKeyDown('S'));
	m_player->SetIsMovingUp(input->IsKeyDown(VK_SPACE));
	m_player->SetIsMovingDown(input->IsKeyDown(VK_SHIFT));
	
	// Switch cameras
	if (input->IsKeyPressed(VK_RIGHT))
	{
		manager->ChangeUp();
		m_player->SetPosition(manager->GetCurrentCamera()->GetPosition());
		m_player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
	}
	if (input->IsKeyPressed(VK_LEFT))
	{
		manager->ChangeDown();
		m_player->SetPosition(manager->GetCurrentCamera()->GetPosition());
		m_player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
	}

	// Create camera
	if (input->IsKeyPressed(VK_RETURN))
	{
		// Create a new camera as a copy of the current one
		manager->CreateCamera();
	}

	// Look at origin
	if (input->IsKeyDown('R'))
	{
		manager->GetCurrentCamera()->SetTarget(0.0f, 0.0f, 0.0f);
	}

	// Turn floor green
	if (input->IsKeyPressed(VK_LBUTTON))
	{
		m_coloredFloor->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Toggle depth tests
	if (input->IsKeyPressed('Z'))
	{
		static bool depthTestOn = true;
		depthTestOn = !depthTestOn;

		switch (depthTestOn)
		{
		case true:
			Direct3D::Get()->EnableZBuffer();
			break;
		case false:
			Direct3D::Get()->DisableZBuffer();
			break;
		}
	}

	// Toggle orthogonal projection
	if (input->IsKeyPressed('O'))
	{
		m_orthogonal = (OrthogonalMode)(1 - m_orthogonal);
	}

	// Change render mode
	if (input->IsKeyPressed(49))	// 1
	{
		m_renderMode = (RenderMode)((m_renderMode + 1) % RenderMode::NR_OF_RENDER_MODES);
	}

	// Change hud mode
	if (input->IsKeyPressed(50))	// 2
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

	// Change number of lights
	if (input->IsKeyPressed(VK_UP))
	{
		m_nrOfLights = min(m_nrOfLights + 1, MAX_NR_OF_LIGHTS);
	}
	if (input->IsKeyPressed(VK_DOWN))
	{
		m_nrOfLights = max(m_nrOfLights - 1, 1);
	}


	// Rotate cubes or camera
	Vector2f mouseMovement = input->MouseMovement();
	
	if (input->IsKeyDown(VK_RBUTTON))
	{
		for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
		{
			m_texturedCubes[i]->Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
		}
	}
	else
	{
		mouseMovement *= 0.015f;

		manager->GetCurrentCamera()->RotateUp(mouseMovement.y);
		manager->GetCurrentCamera()->RotateRight(mouseMovement.x);
		m_player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
	}

	return true;
}

void Game::Update(double dt)
{
	float deltaTime = static_cast<float>(dt);
	static float angle = 0.0f;
	angle += deltaTime;

	m_player->Update(deltaTime);

	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	cam->SetPosition(m_player->GetPosition());
	cam->SetTarget(m_player->GetPosition() + m_player->GetLookDirection());
	PlayerCameraManager::Get()->Update();

	for (unsigned int i = 0; i < m_nrOfLights; i++)
	{
		m_pointLights[i]->SetPosition(Vector3f(std::cosf(angle + i * 0.5f), 1.0f, std::sinf(angle + i * 0.5f)) * 30.0f);
		m_pointLights[i]->SetTarget(Vector3f(0, 0, 0));
		m_pointLights[i]->Update();
	}

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		m_texturedCubes[i]->Update();
	}
	m_coloredFloor->Update();


	m_fpsCounter->Update(deltaTime);
}

void Game::Render()
{
	Direct3D* d3d = Direct3D::Get();

	d3d->ClearDefaultTarget();

	switch (m_renderMode)
	{
	case NORMAL_MODE:
		RenderNormal();
		break;
	case DEPTH_MODE:
		RenderDepth();
		break;
	case DEFERRED_MULTIPLE_LIGHTS_MODE:
		RenderDeferredFirstPass();
		RenderDeferredLightPass();
		break;
	case DEFERRED_MULTIPLE_SHADOW_LIGHTS_MODE:
		RenderDeferredFirstPass();
		RenderMultipleShadowsPass();
		RenderDeferredLightMultipleShadowsPass();
		break;
	default:
		RenderDeferredFirstPass();
		RenderShadowPass();
		RenderDeferredLightPass();
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
	default:
		break;
	}

	RenderHUDText();

	d3d->Present();
}

void Game::RenderNormal()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);
	
	ModelStorage* modelStorage = ModelStorage::Get();
	TextureStorage* textureStorage = TextureStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;
	ID3D11ShaderResourceView* texture;


	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::TEXTURE);

	shaders->SetPerFrameTextureConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix(),
		cam0->GetPosition(),
		1.0f);

	/* ------------------------- Render cubes ------------------------- */
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	texture = textureStorage->GetTexture(m_texturedCubes[0]->GetTextureName());
	textureModel->SetupRender(deviceContext);

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		shaders->SetPerObjectTextureConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix(), texture);
		textureModel->Render(deviceContext);
	}


	/* ========================= Render single color objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::SINGLE_COLOR);

	shaders->SetPerFrameSingleColorConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		cam->GetProjectionMatrix(),
		cam0->GetViewMatrix(),
		cam0->GetProjectionMatrix(),
		cam0->GetPosition(),
		1.0f);

	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor->GetModelName());

	shaders->SetPerObjectSingleColorConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix(), m_coloredFloor->GetColor());
	singleColorModel->SetupRender(deviceContext);
	singleColorModel->Render(deviceContext);
}
void Game::RenderDeferredFirstPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();

	ModelStorage* modelStorage = ModelStorage::Get();
	TextureStorage* textureStorage = TextureStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;
	ID3D11ShaderResourceView* texture;

	d3d->SetDeferredTargets();
	d3d->ClearDeferredTargets();

	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);

	shaders->SetPerFrameDeferredTextureConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	/* ------------------------- Render cubes ------------------------- */
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	texture = textureStorage->GetTexture(m_texturedCubes[0]->GetTextureName());
	textureModel->SetupRender(deviceContext);

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		shaders->SetPerObjectDeferredTextureConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix(), texture);
		textureModel->Render(deviceContext);
	}


	/* ========================= Render single color objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);

	shaders->SetPerFrameDeferredSingleColorConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		cam->GetProjectionMatrix());

	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor->GetModelName());

	shaders->SetPerObjectDeferredSingleColorConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix(), m_coloredFloor->GetColor());
	singleColorModel->SetupRender(deviceContext);
	singleColorModel->Render(deviceContext);
}
void Game::RenderDepth()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();

	ModelStorage* modelStorage = ModelStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::DEPTH);
	
	shaders->SetPerFrameDepthConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	/* ------------------------- Render cubes ------------------------- */
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	textureModel->SetupRender(deviceContext);

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		shaders->SetPerObjectDepthConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		textureModel->Render(deviceContext);
	}

	/* ========================= Render single color objects ========================== */
	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor->GetModelName());

	shaders->SetPerObjectDepthConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix());
	singleColorModel->SetupRender(deviceContext);
	singleColorModel->Render(deviceContext);
}
void Game::RenderShadowPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0); // PlayerCameraManager::Get()->GetCurrentCamera();

	ModelStorage* modelStorage = ModelStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	d3d->SetShadowTarget();
	d3d->ClearShadowTarget();

	/* ========================= Render texture objects ========================== */
	// Set texture shaders, then remove pixel shader
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	shaders->SetPerFrameDeferredShadowConstantBuffer(
		deviceContext,
		cam0->GetViewMatrix(),
		cam0->GetProjectionMatrix());

	/* ------------------------- Render cubes ------------------------- */
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	textureModel->SetupRender(deviceContext);

	for (unsigned int i = 0; i < m_texturedCubes.size(); i++)
	{
		shaders->SetPerObjectDeferredShadowConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		textureModel->Render(deviceContext);
	}

	/* ========================= Render single color objects ========================== */
	// Set color shaders, then remove pixel shader
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor->GetModelName());

	shaders->SetPerObjectDeferredShadowConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix());
	singleColorModel->SetupRender(deviceContext);
	singleColorModel->Render(deviceContext);
}
void Game::RenderMultipleShadowsPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();

	ModelStorage* modelStorage = ModelStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	for (unsigned int i = 0; i < m_nrOfLights; i++)
	{
		m_pointLights[i]->Set(deviceContext);
		m_pointLights[i]->Clear(deviceContext);

		/* ========================= Render texture objects ========================== */
		// Set texture shaders, then remove pixel shader
		shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);
		shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

		shaders->SetPerFrameDeferredShadowConstantBuffer(
			deviceContext,
			m_pointLights[i]->GetView(),
			m_pointLights[i]->GetProjection());

		/* ------------------------- Render cubes ------------------------- */
		textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
		//textureModel->SetupRenderCall(deviceContext);

		for (unsigned int j = 0; j < m_texturedCubes.size(); j++)
		{
			shaders->SetPerObjectDeferredShadowConstantBuffer(deviceContext, m_texturedCubes[j]->GetWorldMatrix());
			textureModel->Render(deviceContext);
		}

		/* ========================= Render single color objects ========================== */
		// Set color shaders, then remove pixel shader
		shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);
		shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

		/* ------------------------- Render floor ------------------------- */
		singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor->GetModelName());
		
		shaders->SetPerObjectDeferredShadowConstantBuffer(deviceContext, m_coloredFloor->GetWorldMatrix());
		singleColorModel->SetupRender(deviceContext);
		singleColorModel->Render(deviceContext);
	}
}
void Game::RenderDeferredLightPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	d3d->SetDefaultTarget();

	if (m_renderMode == RenderMode::DEFERRED_MODE)
	{
		shaders->SetShaderType(deviceContext, ShaderType::D_LIGHT);

		shaders->SetPerFrameDeferredLightShadowConstantBuffer(
			deviceContext,
			cam0->GetViewMatrix(),
			cam0->GetProjectionMatrix(),
			DeferredBufferType::NR_OF_D_ELEMENTS,
			d3d->GetDeferredShaderResourceViews(),
			d3d->GetShadowShaderResourceView(),
			cam0->GetPosition(),
			1.0f);
	}
	else if (m_renderMode == RenderMode::DEFERRED_MULTIPLE_LIGHTS_MODE)
	{
		shaders->SetShaderType(deviceContext, ShaderType::D_MULTIPLE);

		Vector3f lightPos[MAX_NR_OF_LIGHTS];
		float lightIntensity[MAX_NR_OF_LIGHTS];

		unsigned int nCams = PlayerCameraManager::Get()->GetNrOfCameras();
		for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
		{
			lightPos[i] = PlayerCameraManager::Get()->GetCamera(i % nCams)->GetPosition();
			lightIntensity[i] = 1.0f;
		}

		shaders->SetPerFrameDeferredLightMultipleLightsConstantBuffer(
			deviceContext,
			DeferredBufferType::NR_OF_D_ELEMENTS,
			d3d->GetDeferredShaderResourceViews(),
			lightPos,
			lightIntensity);
	}
	else
	{
		shaders->SetShaderType(deviceContext, ShaderType::D_SPLIT);

		shaders->SetPerFrameDeferredLightSplitScreenConstantBuffer(
			deviceContext,
			cam0->GetViewMatrix(),
			cam0->GetProjectionMatrix(),
			DeferredBufferType::NR_OF_D_ELEMENTS,
			d3d->GetDeferredShaderResourceViews(),
			d3d->GetShadowShaderResourceView(),
			cam0->GetPosition(),
			1.0f);
	}

	DeferredScreenTarget::Get()->Render(deviceContext);
}
void Game::RenderDeferredLightMultipleShadowsPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();

	d3d->SetDefaultTarget();

	shaders->SetShaderType(deviceContext, ShaderType::D_MULTIPLE_SHADOWS);

	Vector3f lightPositions[MAX_NR_OF_LIGHTS];
	float lightIntensities[MAX_NR_OF_LIGHTS];
	DirectX::XMMATRIX lightViewMatrices[MAX_NR_OF_LIGHTS];
	DirectX::XMMATRIX lightProjectionMatrices[MAX_NR_OF_LIGHTS];
	ID3D11ShaderResourceView* lightDepthTextures[MAX_NR_OF_LIGHTS];

	for (unsigned int i = 0; i < m_nrOfLights; i++)
	{
		lightPositions[i] = m_pointLights[i]->GetPosition();
		lightIntensities[i] = 1.0f;
		lightViewMatrices[i] = m_pointLights[i]->GetView();
		lightProjectionMatrices[i] = m_pointLights[i]->GetProjection();
		lightDepthTextures[i] = m_pointLights[i]->GetResource();
	}

	shaders->SetPerFrameDeferredLightMultipleShadowLightsConstantBuffer(
		deviceContext,
		DeferredBufferType::NR_OF_D_ELEMENTS,
		d3d->GetDeferredShaderResourceViews(),
		m_nrOfLights,
		lightDepthTextures,
		lightPositions,
		lightViewMatrices,
		lightProjectionMatrices,
		lightIntensities);

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
void Game::RenderHUDText()
{
	Direct3D::Get()->DisableZBuffer();

	m_fpsCounter->Render();

	Direct3D::Get()->EnableZBuffer();
	Direct3D::Get()->SetDefaultBlendState();
}