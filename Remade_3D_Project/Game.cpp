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

#include "SingleColorObject.hpp"
#include "TextureObject.hpp"

#include "SingleColorModel.hpp"
#include "TextureModel.hpp"

#include "ModelStorage.hpp"
#include "TextureStorage.hpp"
#include "ShaderStorage.hpp"
#include "SamplerStorage.hpp"
#include "ConstantBufferStorage.hpp"

#include "PointLightManager.hpp"
#include "LightSettings.hpp"

#include "OBB.hpp"
#include "Collision.hpp"

#include "ChunkRenderSettings.hpp"

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
	PointLightManager::Delete();

	ModelStorage::Delete();
	TextureStorage::Delete();
	ShaderStorage::Delete();
	SamplerStorage::Delete();
	ConstantBufferStorage::Delete();

	delete[] m_texturedCubes;
	/*for (unsigned int i = 0; i < m_nrOfCubes; i++)
	{
		delete m_texturedCubes[i];
	}*/
}

bool Game::Initialize()
{
	PointLightManager* lightManager;

	if (!Window::Get()->Initialize(Vector2i(WNDW, WNDH)))
		return false;
	if (!Direct3D::Get()->Initialize(Window::Get()))
		return false;
	if (!ShaderManager::Get()->Initialize(Direct3D::Get()->GetDevice()))
		return false;
	if (!DeferredScreenTarget::Get()->Initialize(Direct3D::Get()->GetDevice()))
		return false;


	/* ============================================= Cameras ============================================= */
	if (!PlayerCameraManager::Get()->Initialize())
		return false;

	Camera* cam = PlayerCameraManager::Get()->CreateCamera();
	cam->SetDimensions(Window::Get()->GetDimensions());
	cam->SetPosition(Vector3f(0, 50, -100));
	cam->SetTarget(Vector3f(0, 0, 0));
	cam->Update();


	
	/* ============================================= Storages ============================================= */
	if (!ModelStorage::Get()->LoadTextureModel(Direct3D::Get()->GetDevice(), "../Models/cube_uv.obj"))
		return false;
	if (!ModelStorage::Get()->LoadSingleColorModel(Direct3D::Get()->GetDevice(), "../Models/cube.obj"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "../Textures/Torgue.png"))
		return false;
	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "../Textures/BrickWallRaw.jpg"))
		return false;
	if (!SamplerStorage::Get()->Initialize(Direct3D::Get()->GetDevice()))
		return false;
	if (!ConstantBufferStorage::Get()->Initialize(Direct3D::Get()->GetDevice()))
		return false;

	/* ============================================= Objects ============================================= */
	// Cubes
	unsigned int nX = 20, nY = 25, nZ = 20;
	m_nrOfCubes = nX * nY * nZ;
	m_texturedCubes = new TextureObject[m_nrOfCubes];

	float distance = 4;

	Vector3f startPos = Vector3f((nX - 1) * -0.5f, 1.0f, (nZ - 1) * -0.5f) * distance;

	for (unsigned int z = 0; z < nZ; z++)
	{
		for (unsigned int y = 0; y < nY; y++)
		{
			for (unsigned int x = 0; x < nX; x++)
			{
				int index = x + y * nX + z * nX * nY;

				//m_texturedCubes[index] = new TextureObject;
				//if (!m_texturedCubes[index]->Initialize("../Models/cube_uv.obj", "../Textures/Torgue.png"))
				if (!m_texturedCubes[index].Initialize("../Models/cube_uv.obj", "../Textures/Torgue.png"))
					return false;

				//m_texturedCubes[index]->SetPosition(startPos + Vector3f(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) * distance);
				m_texturedCubes[index].SetPosition(startPos + Vector3f(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) * distance);

				/*m_texturedCubes.push_back(std::make_unique<TextureObject>());
				if (!m_texturedCubes.back().get()->Initialize("../Models/cube_uv.obj", "../Textures/Torgue.png"))
					return false;

				m_texturedCubes.back().get()->SetPosition(startPos + Vector3f(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) * distance);*/
			}
		}
	}

	// Floor
	m_coloredFloor = std::make_unique<SingleColorObject>();
	if (!m_coloredFloor->Initialize("../Models/cube.obj", Vector3f(1, 0, 0)))
		return false;

	m_coloredFloor->SetScale(100.0f, 0.02f, 100.0f);
	m_coloredFloor->SetPosition(0.0f, 0.0f, 0.0f);

	// HUD
	m_HUDObject = std::make_unique<HUDObject>();
	if (!m_HUDObject->Initialize(Direct3D::Get()->GetDevice(), "../Textures/Torgue.png", Vector2i(0, 0), Vector2i(400, 400)))
		return false;
	m_HUDObject->SetPosition(Vector2i(500, 500));
	m_HUDObject->SetDimensions(Vector2i(200, 200));


	lightManager = PointLightManager::Get();
	lightManager->AddPointLight();
	PointLight* light = lightManager->GetPointLight(0);

	if (!light->Initialize(Direct3D::Get()->GetDevice(), Window::Get()->GetDimensions() * 4/*Vector2f(static_cast<float>(WNDW), static_cast<float>(WNDH)) * 4.0f*/))
		return false;
	light->SetPosition(Vector3f(-40.0f, 20.0f, -20.0f));
	light->SetTarget(Vector3f(0.0f, 0.0f, 0.0f));
	light->Update();

	/*for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
	{
		m_pointLights[i] = std::make_unique<PointLight>();
		if (!m_pointLights[i]->Initialize(Direct3D::Get()->GetDevice(), Vector2f(static_cast<float>(WNDW), static_cast<float>(WNDH)) * 4.0f))
			return false;
		m_pointLights[i]->SetPosition(Vector3f(-40.0f + 20.0f * i, 20.0f, -20.0f));
		m_pointLights[i]->SetTarget(Vector3f(0.0f, 0.0f, 0.0f));
		
		m_pointLights[i]->Update();
	}
	m_nrOfLights = 1;*/


	m_fpsCounter = std::make_unique<FPSCounter>();
	m_fpsCounter->Initialize(Direct3D::Get()->GetDevice(), Direct3D::Get()->GetDeviceContext());


	m_renderMode = RenderMode::DEFERRED_CHUNK_MODE;
	m_HUDMode = HUDMode::HUD_OFF;
	m_orthogonal = OrthogonalMode::ORTHOGONAL_OFF;

	m_player = std::make_unique<Character>();
	m_player->SetMovementSpeed(20.0f);
	m_player->SetPosition(PlayerCameraManager::Get()->GetCurrentCamera()->GetPosition());
	m_player->SetLookDirection(PlayerCameraManager::Get()->GetCurrentCamera()->GetTargetDirection());


	MapProjectionMatrix();

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

			long long nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
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

	if (input->IsKeyPressed(VK_LBUTTON))
	{
		CubeIntersection();

		/*Vector2f mousePos = input->MousePosition();
		CreatePickingVector(mousePos.x, mousePos.y);*/
	}

	// Change cube texture and turn floor green
	if (input->IsKeyPressed('T'))
	{
		static bool toggle = false;
		toggle = !toggle;

		if (toggle)
		{
			//m_texturedCubes[0].get()->SetTextureName("../Textures/BrickWallRaw.jpg");
			//m_texturedCubes[0]->SetTextureName("../Textures/BrickWallRaw.jpg");
			m_texturedCubes[0].SetTextureName("../Textures/BrickWallRaw.jpg");
			m_coloredFloor->SetColor(0.0f, 1.0f, 0.0f);
		}
		else
		{
			//m_texturedCubes[0].get()->SetTextureName("../Textures/Torgue.png");
			//m_texturedCubes[0]->SetTextureName("../Textures/Torgue.png");
			m_texturedCubes[0].SetTextureName("../Textures/Torgue.png");
			m_coloredFloor->SetColor(1.0f, 0.0f, 0.0f);
		}
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

		MapProjectionMatrix();
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
		if (PointLightManager::Get()->AddPointLight())
			PointLightManager::Get()->GetPointLight(PointLightManager::Get()->GetNrOfPointLights() - 1)->Initialize(Direct3D::Get()->GetDevice(), Window::Get()->GetDimensions() * 4);
	}
	if (input->IsKeyPressed(VK_DOWN))
	{
		PointLightManager::Get()->RemovePointLight(PointLightManager::Get()->GetNrOfPointLights() - 1);
	}


	// Rotate cubes or camera
	Vector2f mouseMovement = input->MouseMovement();
	
	if (input->IsKeyDown(VK_RBUTTON))
	{
		if (mouseMovement != Vector2f(0, 0))
		{
			unsigned int nrOfCubes = m_nrOfCubes;//m_texturedCubes.size();
			for (unsigned int i = 0; i < nrOfCubes; i++)
			{
				//m_texturedCubes[i].get()->Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
				//m_texturedCubes[i]->Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
				m_texturedCubes[i].Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
			}
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
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	PointLightManager* lightManager = PointLightManager::Get();
	unsigned int n;

	angle += deltaTime;

	m_player->Update(deltaTime);

	cam->SetPosition(m_player->GetPosition());
	cam->SetTarget(m_player->GetPosition() + m_player->GetLookDirection());
	PlayerCameraManager::Get()->Update();

	n = lightManager->GetNrOfPointLights();
	for (unsigned int i = 0; i < n; i++)
	{
		lightManager->GetPointLight(i)->SetPosition(Vector3f(std::cosf(angle + i * 0.5f), 1.0f, std::sinf(angle + i * 0.5f)) * 100.0f);
		lightManager->GetPointLight(i)->SetTarget(Vector3f(0, 0, 0));
		lightManager->GetPointLight(i)->Update();
	}

	n = m_nrOfCubes;//m_texturedCubes.size();
	for (unsigned int i = 0; i < n; i++)
	{
		//m_texturedCubes[i].get()->Update();
		//m_texturedCubes[i]->Update();
		m_texturedCubes[i].Update();
	}
	m_coloredFloor->Update();

	m_fpsCounter->Update(deltaTime);

	ConstantBufferStorage::Get()->SetVSViewMatrix(Direct3D::Get()->GetDeviceContext(), cam->GetViewMatrix());
}
void Game::MapProjectionMatrix()
{
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	ConstantBufferStorage* storage = ConstantBufferStorage::Get();
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	storage->SetVSProjectionMatrix(deviceContext, m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());
}

//void Game::CreatePickingVector(float x, float y)
//{
//	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
//
//	DirectX::XMMATRIX proj = cam->GetProjectionMatrix();
//	DirectX::XMMATRIX view = cam->GetViewMatrix();
//
//	DirectX::XMVECTOR projDet = DirectX::XMMatrixDeterminant(proj);
//	DirectX::XMVECTOR viewDet = DirectX::XMMatrixDeterminant(view);
//
//	DirectX::XMMATRIX projInv = DirectX::XMMatrixInverse(&projDet, proj);
//	DirectX::XMMATRIX viewInv = DirectX::XMMatrixInverse(&viewDet, view);
//
//
//	// [0, 1]
//	x /= WNDW;
//	y /= WNDH;
//
//	// [-1, 1]
//	x = x * 2 - 1;
//	y = -(y * 2 - 1);
//
//	/*x = x / proj.r[0].m128_f32[0];
//	y = y / proj.r[1].m128_f32[1];*/
//
//	DirectX::XMVECTOR vecNear = { x, y, 0 };
//	DirectX::XMVECTOR vecFar = { x, y, 1 };
//
//	DirectX::XMMATRIX vp = DirectX::XMMatrixMultiply(view, proj);
//	DirectX::XMVECTOR vpDet = DirectX::XMMatrixDeterminant(vp);
//	DirectX::XMMATRIX vpInv = DirectX::XMMatrixInverse(&vpDet, vp);
//
//	
//	DirectX::XMVECTOR orig = DirectX::XMVector3Transform(vecNear, vpInv);
//	DirectX::XMVECTOR end = DirectX::XMVector3Transform(vecFar, vpInv);
//
//	Vector3f origin(orig.m128_f32[0], orig.m128_f32[1], orig.m128_f32[2]);
//	Vector3f endP(end.m128_f32[0], end.m128_f32[1], end.m128_f32[2]);
//
//	Vector3f dir = (endP - origin).normalized();
//
//	Vector3f camPos = cam->GetPosition();
//
//	/*DirectX::XMVECTOR dir = { x, y, 1 };
//	DirectX::XMVECTOR orig = { 0, 0, 0 };
//
//	dir = DirectX::XMVector3Transform(dir, viewInv);
//	orig = DirectX::XMVector3Transform(orig, viewInv);*/
//
//
//
//
//
//	/*direction.x = -((x * viewInv.r[0].m128_f32[0]) + (y * viewInv.r[1].m128_f32[0]) + viewInv.r[2].m128_f32[0]);
//	direction.y = -((x * viewInv.r[0].m128_f32[1]) + (y * viewInv.r[1].m128_f32[1]) + viewInv.r[2].m128_f32[1]);
//	direction.z = (x * viewInv.r[0].m128_f32[2]) + (y * viewInv.r[1].m128_f32[2]) + viewInv.r[2].m128_f32[2];*/
//
//	//Vector3f camPos = cam->GetPosition();
//	//Vector3f origin(orig.m128_f32[0], orig.m128_f32[1], orig.m128_f32[2]);
//
//
//	//m_texturedCubes[0]->SetPosition(origin + direction * 10);
//
//
//	/*DirectX::XMVECTOR pos = { screenPos.x, screenPos.y, 0 };
//
//	pos = DirectX::XMVector3Transform(pos, projInv);
//	pos = DirectX::XMVector3Transform(pos, viewInv);
//
//	Vector3f camPos = cam->GetPosition();
//
//	DirectX::XMVECTOR camPos2 = { camPos.x, camPos.y, camPos.z };
//	camPos2 = DirectX::XMVector3Transform(camPos2, view);
//	camPos2 = DirectX::XMVector3Transform(camPos2, proj);
//
//	camPos2.m128_f32[0] /= camPos2.m128_f32[3];
//	camPos2.m128_f32[1] /= camPos2.m128_f32[3];
//	camPos2.m128_f32[2] /= camPos2.m128_f32[3];
//	camPos2.m128_f32[3] /= camPos2.m128_f32[3];*/
//
//}

void Game::CubeIntersection()
{
	OBB obb;
	obb.halfSides[0] = 1;
	obb.halfSides[1] = 1;
	obb.halfSides[2] = 1;
	obb.vectors[0] = Vector3f(1, 0, 0);
	obb.vectors[1] = Vector3f(0, 1, 0);
	obb.vectors[2] = Vector3f(0, 0, 1);

	Vector3f origin = m_player->GetPosition();
	Vector3f direction = m_player->GetLookDirection();

	IntersectionData data;

	//obb.center = m_texturedCubes[0]->GetPosition();
	obb.center = m_texturedCubes[0].GetPosition();
	data = RayVsOBB(origin, direction, obb);
	if (data.intersection)
	{
		//m_texturedCubes[0]->SetTextureName("../Textures/BrickWallRaw.jpg");
		m_texturedCubes[0].SetTextureName("../Textures/BrickWallRaw.jpg");
	}
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
	case DEFERRED_MODE:
		RenderDeferredFirstPass();
		RenderShadowPass();
		RenderDeferredLightShadowPass();
		break;
	case DEFERRED_CHUNK_MODE:
		RenderDeferredFirstPassChunks();
		RenderShadowPassChunks();
		RenderDeferredLightShadowPass();
		break;
	case DEFERRED_SPLIT_MODE:
		RenderDeferredFirstPass();
		RenderShadowPass();
		RenderDeferredLightSplitPass();
		break;
	case DEFERRED_MULTIPLE_LIGHTS_MODE:
		RenderDeferredFirstPass();
		RenderDeferredLightMultiplePass();
		break;
	case DEFERRED_MULTIPLE_SHADOW_LIGHTS_MODE:
		RenderDeferredFirstPass();
		RenderMultipleShadowsPass();
		RenderDeferredLightMultipleShadowsPass();
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
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;
	ID3D11ShaderResourceView* texture;

	unsigned int nrOfCubes = m_nrOfCubes;//m_texturedCubes.size();

	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::TEXTURE);

	bufferStorage->SetVSPointLight(deviceContext, cam0->GetPosition(), 1.0f);

	/* ------------------------- Render cubes ------------------------- */
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].get()->GetModelName());
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].GetModelName());
	textureModel->SetupRender(deviceContext);

	//texture = textureStorage->GetTexture(m_texturedCubes[0].get()->GetTextureName());
	//texture = textureStorage->GetTexture(m_texturedCubes[0]->GetTextureName());
	texture = textureStorage->GetTexture(m_texturedCubes[0].GetTextureName());
	shaders->SetPerObjectTextureConstantBuffer(deviceContext, texture);

	for (unsigned int i = 0; i < nrOfCubes; i++)
	{
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i].get()->GetWorldMatrix());
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i].GetWorldMatrix());
		textureModel->Render(deviceContext);
	}


	/* ========================= Render single color objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::SINGLE_COLOR);

	bufferStorage->SetPSPointLight(deviceContext, cam0->GetPosition(), 1.0f);

	/* ------------------------- Render floor ------------------------- */

	bufferStorage->SetVSWorldMatrix(deviceContext, m_coloredFloor.get()->GetWorldMatrix());
	bufferStorage->SetVSColor(deviceContext, m_coloredFloor.get()->GetColor());

	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor.get()->GetModelName());
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
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;
	ID3D11ShaderResourceView* texture;

	unsigned int n;

	d3d->SetDeferredTargets();
	d3d->ClearDeferredTargets();

	bufferStorage->SetVSViewMatrix(deviceContext, cam->GetViewMatrix());
	bufferStorage->SetVSProjectionMatrix(deviceContext, m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);

	/* ------------------------- Render cubes ------------------------- */
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].get()->GetModelName());
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].GetModelName());
	textureModel->SetupRender(deviceContext);

	//texture = textureStorage->GetTexture(m_texturedCubes[0].get()->GetTextureName());
	//texture = textureStorage->GetTexture(m_texturedCubes[0]->GetTextureName());
	texture = textureStorage->GetTexture(m_texturedCubes[0].GetTextureName());
	shaders->SetPerObjectDeferredTextureConstantBuffer(deviceContext, texture);

	n = m_nrOfCubes;// m_texturedCubes.size();
	for (unsigned int i = 0; i < n; i++)
	{
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i].get()->GetWorldMatrix());
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i].GetWorldMatrix());
		textureModel->Render(deviceContext);
	}


	/* ========================= Render single color objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);

	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor.get()->GetModelName());
	singleColorModel->SetupRender(deviceContext);

	bufferStorage->SetVSWorldMatrix(deviceContext, m_coloredFloor.get()->GetWorldMatrix());
	bufferStorage->SetVSColor(deviceContext, m_coloredFloor.get()->GetColor());
	
	singleColorModel->Render(deviceContext);
}
void Game::RenderDeferredFirstPassChunks()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();

	ModelStorage* modelStorage = ModelStorage::Get();
	TextureStorage* textureStorage = TextureStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;
	ID3D11ShaderResourceView* texture;

	d3d->SetDeferredTargets();
	d3d->ClearDeferredTargets();

	bufferStorage->SetGSViewMatrix(deviceContext, cam->GetViewMatrix());
	bufferStorage->SetGSProjectionMatrix(deviceContext, m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE_CHUNK);

	/* ------------------------- Render cubes ------------------------- */
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].get()->GetModelName());
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].GetModelName());
	textureModel->SetupRender(deviceContext);

	//texture = textureStorage->GetTexture(m_texturedCubes[0].get()->GetTextureName());
	//texture = textureStorage->GetTexture(m_texturedCubes[0]->GetTextureName());
	texture = textureStorage->GetTexture(m_texturedCubes[0].GetTextureName());
	shaders->SetPerObjectDeferredTextureChunkShaderGroup(deviceContext, texture);

	DirectX::XMMATRIX worldMatrices[CHUNK_SIZE];
	
	//int nrOfFullRenderPasses = m_texturedCubes.size() / CHUNK_SIZE;
	unsigned int nrOfFullRenderPasses = m_nrOfCubes / CHUNK_SIZE;
	unsigned int nrOfCubesInFullRenderPasses = nrOfFullRenderPasses * CHUNK_SIZE;
	//int nrOfRemainingCubes = m_texturedCubes.size() - nrOfCubesInFullRenderPasses;
	unsigned int nrOfRemainingCubes = m_nrOfCubes - nrOfCubesInFullRenderPasses;

	// Render the full chunks of objects
	for (unsigned int i = 0; i < nrOfFullRenderPasses; i++)
	{
		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
		{
			//worldMatrices[j] = m_texturedCubes[i * CHUNK_SIZE + j].get()->GetWorldMatrix();
			//worldMatrices[j] = m_texturedCubes[i * CHUNK_SIZE + j]->GetWorldMatrix();
			worldMatrices[j] = m_texturedCubes[i * CHUNK_SIZE + j].GetWorldMatrix();
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, CHUNK_SIZE);
		textureModel->Render(deviceContext);
	}

	// Render the leftovers
	if (nrOfRemainingCubes > 0)
	{
		for (unsigned int i = 0; i < nrOfRemainingCubes; i++)
		{
			//worldMatrices[i] = m_texturedCubes[nrOfCubesInFullRenderPasses + i].get()->GetWorldMatrix();
			//worldMatrices[i] = m_texturedCubes[nrOfCubesInFullRenderPasses + i]->GetWorldMatrix();
			worldMatrices[i] = m_texturedCubes[nrOfCubesInFullRenderPasses + i].GetWorldMatrix();
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, nrOfRemainingCubes);
		textureModel->Render(deviceContext);
	}
	

	/* ========================= Render single color objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);

	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor.get()->GetModelName());
	singleColorModel->SetupRender(deviceContext);

	bufferStorage->SetVSWorldMatrix(deviceContext, m_coloredFloor.get()->GetWorldMatrix());
	bufferStorage->SetVSColor(deviceContext, m_coloredFloor.get()->GetColor());

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

	unsigned int n;

	shaders->SetPerFrameDepthConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::DEPTH);

	/* ------------------------- Render cubes ------------------------- */
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].get()->GetModelName());
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].GetModelName());
	textureModel->SetupRender(deviceContext);

	n = m_nrOfCubes;//m_texturedCubes.size();
	for (unsigned int i = 0; i < n; i++)
	{
		//shaders->SetPerObjectDepthConstantBuffer(deviceContext, m_texturedCubes[i].get()->GetWorldMatrix());
		//shaders->SetPerObjectDepthConstantBuffer(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		shaders->SetPerObjectDepthConstantBuffer(deviceContext, m_texturedCubes[i].GetWorldMatrix());
		textureModel->Render(deviceContext);
	}

	/* ========================= Render single color objects ========================== */
	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor.get()->GetModelName());

	shaders->SetPerObjectDepthConstantBuffer(deviceContext, m_coloredFloor.get()->GetWorldMatrix());
	singleColorModel->SetupRender(deviceContext);
	singleColorModel->Render(deviceContext);
}
void Game::RenderShadowPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	unsigned int n;

	d3d->SetShadowTarget();
	d3d->ClearShadowTarget();

	bufferStorage->SetVSViewMatrix(deviceContext, cam0->GetViewMatrix());
	bufferStorage->SetVSProjectionMatrix(deviceContext, m_orthogonal ? cam0->GetOrthogonalMatrix() : cam0->GetProjectionMatrix());

	/* ========================= Render texture objects ========================== */
	// Set texture shaders, then remove pixel shader
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	/* ------------------------- Render cubes ------------------------- */
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].get()->GetModelName());
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].GetModelName());
	textureModel->SetupRender(deviceContext);

	n = m_nrOfCubes;// m_texturedCubes.size();
	for (unsigned int i = 0; i < n; i++)
	{
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i].get()->GetWorldMatrix());
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i].GetWorldMatrix());
		textureModel->Render(deviceContext);
	}

	/* ========================= Render single color objects ========================== */
	// Set color shaders, then remove pixel shader
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor.get()->GetModelName());

	bufferStorage->SetVSWorldMatrix(deviceContext, m_coloredFloor.get()->GetWorldMatrix());
	singleColorModel->SetupRender(deviceContext);
	singleColorModel->Render(deviceContext);
}
void Game::RenderShadowPassChunks()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	d3d->SetShadowTarget();
	d3d->ClearShadowTarget();

	bufferStorage->SetGSViewMatrix(deviceContext, cam0->GetViewMatrix());
	bufferStorage->SetGSProjectionMatrix(deviceContext, m_orthogonal ? cam0->GetOrthogonalMatrix() : cam0->GetProjectionMatrix());

	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE_CHUNK);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	/* ------------------------- Render cubes ------------------------- */
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].get()->GetModelName());
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].GetModelName());
	textureModel->SetupRender(deviceContext);

	DirectX::XMMATRIX worldMatrices[CHUNK_SIZE];

	//int nrOfFullRenderPasses = m_texturedCubes.size() / CHUNK_SIZE;
	unsigned int nrOfFullRenderPasses = m_nrOfCubes / CHUNK_SIZE;
	unsigned int nrOfCubesInFullRenderPasses = nrOfFullRenderPasses * CHUNK_SIZE;
	//int nrOfRemainingCubes = m_texturedCubes.size() - nrOfCubesInFullRenderPasses;
	unsigned int nrOfRemainingCubes = m_nrOfCubes - nrOfCubesInFullRenderPasses;

	// Render the full chunks of objects
	for (unsigned int i = 0; i < nrOfFullRenderPasses; i++)
	{
		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
		{
			//worldMatrices[j] = m_texturedCubes[i * CHUNK_SIZE + j].get()->GetWorldMatrix();
			//worldMatrices[j] = m_texturedCubes[i * CHUNK_SIZE + j]->GetWorldMatrix();
			worldMatrices[j] = m_texturedCubes[i * CHUNK_SIZE + j].GetWorldMatrix();
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, CHUNK_SIZE);
		textureModel->Render(deviceContext);
	}

	// Render the leftovers
	if (nrOfRemainingCubes > 0)
	{
		for (unsigned int i = 0; i < nrOfRemainingCubes; i++)
		{
			//worldMatrices[i] = m_texturedCubes[nrOfCubesInFullRenderPasses + i].get()->GetWorldMatrix();
			//worldMatrices[i] = m_texturedCubes[nrOfCubesInFullRenderPasses + i]->GetWorldMatrix();
			worldMatrices[i] = m_texturedCubes[nrOfCubesInFullRenderPasses + i].GetWorldMatrix();
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, nrOfRemainingCubes);
		textureModel->Render(deviceContext);
	}

	/* ========================= Render single color objects ========================== */
	// Set color shaders, then remove pixel shader
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor.get()->GetModelName());

	bufferStorage->SetVSWorldMatrix(deviceContext, m_coloredFloor.get()->GetWorldMatrix());
	singleColorModel->SetupRender(deviceContext);
	singleColorModel->Render(deviceContext);
}
void Game::RenderMultipleShadowsPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();

	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	PointLightManager* lightManager = PointLightManager::Get();

	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	PointLight* pointLight;

	unsigned int nrOfCubes = m_nrOfCubes;// m_texturedCubes.size();

	for (unsigned int i = 0; i < lightManager->GetNrOfPointLights(); i++)
	{
		pointLight = lightManager->GetPointLight(i);

		pointLight->Set(deviceContext);
		pointLight->Clear(deviceContext);

		bufferStorage->SetVSViewMatrix(deviceContext, pointLight->GetView());
		bufferStorage->SetVSProjectionMatrix(deviceContext, pointLight->GetProjection());

		/* ========================= Render texture objects ========================== */
		// Set texture shaders, then remove pixel shader
		shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);
		shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

		/* ------------------------- Render cubes ------------------------- */
		//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].get()->GetModelName());
		//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
		textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].GetModelName());
		textureModel->SetupRender(deviceContext);

		for (unsigned int j = 0; j < nrOfCubes; j++)
		{
			//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[j].get()->GetWorldMatrix());
			//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[j]->GetWorldMatrix());
			bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[j].GetWorldMatrix());
			textureModel->Render(deviceContext);
		}

		/* ========================= Render single color objects ========================== */
		// Set color shaders, then remove pixel shader
		shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);
		shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

		/* ------------------------- Render floor ------------------------- */
		singleColorModel = modelStorage->GetSingleColorModel(m_coloredFloor.get()->GetModelName());
		singleColorModel->SetupRender(deviceContext);
		
		bufferStorage->SetVSWorldMatrix(deviceContext, m_coloredFloor.get()->GetWorldMatrix());
		singleColorModel->Render(deviceContext);
	}
}

void Game::RenderDeferredLightShadowPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	d3d->SetDefaultTarget();

	shaders->SetShaderType(deviceContext, ShaderType::D_LIGHT);

	bufferStorage->SetPSViewMatrix(deviceContext, cam0->GetViewMatrix());
	bufferStorage->SetPSProjectionMatrix(deviceContext, cam0->GetProjectionMatrix());
	bufferStorage->SetPSPointLight(deviceContext, cam0->GetPosition(), 1.0f);

	shaders->SetPerFrameDeferredLightShadowConstantBuffer(
		deviceContext,
		DeferredBufferType::NR_OF_D_ELEMENTS,
		d3d->GetDeferredShaderResourceViews(),
		d3d->GetShadowShaderResourceView());

	DeferredScreenTarget::Get()->Render(deviceContext);
}
void Game::RenderDeferredLightSplitPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	d3d->SetDefaultTarget();

	shaders->SetShaderType(deviceContext, ShaderType::D_SPLIT);

	bufferStorage->SetPSViewMatrix(deviceContext, cam0->GetViewMatrix());
	bufferStorage->SetPSProjectionMatrix(deviceContext, cam0->GetProjectionMatrix());
	bufferStorage->SetPSPointLight(deviceContext, cam0->GetPosition(), 1.0f);

	shaders->SetPerFrameDeferredLightSplitScreenConstantBuffer(
		deviceContext,
		DeferredBufferType::NR_OF_D_ELEMENTS,
		d3d->GetDeferredShaderResourceViews(),
		d3d->GetShadowShaderResourceView());

	DeferredScreenTarget::Get()->Render(deviceContext);
}
void Game::RenderDeferredLightMultiplePass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	PlayerCameraManager* cameraManager = PlayerCameraManager::Get();
	Camera* cam0 = cameraManager->GetCamera(0);

	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	Vector3f lightPos[MAX_NR_OF_LIGHTS];
	float lightIntensity[MAX_NR_OF_LIGHTS];


	d3d->SetDefaultTarget();

	shaders->SetShaderType(deviceContext, ShaderType::D_MULTIPLE);

	unsigned int nCams = cameraManager->GetNrOfCameras();
	for (unsigned int i = 0; i < MAX_NR_OF_LIGHTS; i++)
	{
		lightPos[i] = cameraManager->GetCamera(i % nCams)->GetPosition();
		lightIntensity[i] = 1.0f;
	}

	bufferStorage->SetPSPointLightArray(deviceContext, lightPos, lightIntensity);

	shaders->SetPerFrameDeferredLightMultipleLightsConstantBuffer(
		deviceContext,
		DeferredBufferType::NR_OF_D_ELEMENTS,
		d3d->GetDeferredShaderResourceViews());

	DeferredScreenTarget::Get()->Render(deviceContext);
}
void Game::RenderDeferredLightMultipleShadowsPass()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	PointLightManager* lightManager = PointLightManager::Get();

	PointLight* pointLight;

	Vector3f lightPositions[MAX_NR_OF_LIGHTS];
	float lightIntensities[MAX_NR_OF_LIGHTS];
	DirectX::XMMATRIX lightViewMatrices[MAX_NR_OF_LIGHTS];
	DirectX::XMMATRIX lightProjectionMatrices[MAX_NR_OF_LIGHTS];
	ID3D11ShaderResourceView* lightDepthTextures[MAX_NR_OF_LIGHTS];


	d3d->SetDefaultTarget();

	shaders->SetShaderType(deviceContext, ShaderType::D_MULTIPLE_SHADOWS);

	for (unsigned int i = 0; i < lightManager->GetNrOfPointLights(); i++)
	{
		pointLight = lightManager->GetPointLight(i);

		lightPositions[i] = pointLight->GetPosition();
		lightIntensities[i] = 1.0f;
		lightViewMatrices[i] = pointLight->GetView();
		lightProjectionMatrices[i] = pointLight->GetProjection();
		lightDepthTextures[i] = pointLight->GetResource();
	}

	bufferStorage->SetPSPointLightArray(deviceContext, lightPositions, lightIntensities);
	bufferStorage->SetPSViewMatrixArray(deviceContext, lightViewMatrices);
	bufferStorage->SetPSProjectionMatrixArray(deviceContext, lightProjectionMatrices);
	bufferStorage->SetPSNrOfLights(deviceContext, lightManager->GetNrOfPointLights());

	shaders->SetPerFrameDeferredLightMultipleShadowLightsConstantBuffer(
		deviceContext,
		DeferredBufferType::NR_OF_D_ELEMENTS,
		d3d->GetDeferredShaderResourceViews(),
		lightDepthTextures);

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

	m_HUDObject.get()->Render(deviceContext);

	// Reenable
	d3d->EnableZBuffer();
}
void Game::RenderHUDText()
{
	Direct3D* d3d = Direct3D::Get();

	d3d->DisableZBuffer();

	m_fpsCounter.get()->Render();

	d3d->EnableZBuffer();
	d3d->SetDefaultBlendState();
}