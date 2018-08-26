#include "PlayState.hpp"

#include "../Engine/Events/EventDispatcher.hpp"

//#include "../Engine/Input/Input.hpp"

#include "../Engine/Core/Engine.hpp"
//#include "../Engine/Render/Shaders/ShaderManager.hpp"
//#include "../Engine/Render/Shaders/ShaderStorage.hpp"
//#include "../Engine/Render/DeferredScreenTarget.hpp"
//#include "../Engine/Render/SamplerStorage.hpp"
//#include "../Engine/Render/WindowSettings.hpp"
//#include "../Engine/Render/ChunkRenderSettings.hpp"
//#include "../Engine/Render/RenderManager.hpp"
//#include "../Engine/GUI/FPS_Counter.hpp"
//#include "../Engine/Objects/Models/ModelStorage.hpp"
//#include "../Engine/Objects/Textures/TextureStorage.hpp"
//#include "../Engine/Buffers/ConstantBufferStorage.hpp"

#include "../Engine/Lights/PointLightManager.hpp"
#include "../Engine/Lights/PointLight.hpp"
#include "../Engine/Lights/LightSettings.hpp"


#include "../Engine/Camera/Camera.hpp"
//#include "../Engine/Camera/PlayerCameraManager.hpp"
//#include "../Engine/FrameWork/FrameWork.hpp"

#include "../Engine/Objects/Objects/SingleColorObject.hpp"
#include "../Engine/Objects/Objects/TextureObject.hpp"

#include "../Engine/Objects/Models/SingleColorModel.hpp"
#include "../Engine/Objects/Models/TextureModel.hpp"

//#include "../Engine/GUI/HUDTexture.hpp"

#include "../Engine/Math/OBB.hpp"
#include "../Engine/Math/Collision.hpp"

#include "Character.hpp"
#include "SceneStorage.hpp"
#include "Scene.hpp"

//#include "QuadTree.hpp"

//#define OLD_MODE
//#define HYBRID_MODE

PlayState::PlayState(StateMachineV2<GameState>* stateMachine) : GameState::GameState(stateMachine)
{
}

PlayState::~PlayState()
{
	//delete[] m_texturedCubes;
}

void PlayState::OnEntry()
{
}

void PlayState::OnExit()
{
}

bool PlayState::Initialize()
{
#ifdef HYBRID_MODE
	m_fpsCounter = std::make_unique<FPSCounter>();
	m_fpsCounter->Initialize();
#elif defined OLD_MODE

	PointLightManager* lightManager;
	Camera* cam;

	if (!SceneStorage::Get()->LoadScene("Scene1"))
		return false;
	
	m_scene = SceneStorage::Get()->GetScene("Scene1");
	m_scene->LoadIntoRenderManager();

	/* =========================================== QuadTree =================================== */
	/*m_quadTree = std::make_unique<QuadTree>();
	m_quadTree.get()->Create(Vector3f(0, 10, 0), Vector3f(200, 10, 200));*/


	// /* ============================================= Objects ============================================= */
	// /* ------------------------------------------- Cubes ---------------------------------------------- */
	//
	//
	////unsigned int nX = 20, nY = 25, nZ = 20;
	//unsigned int nX = 10, nY = 1, nZ = 10;
	//
	////m_nrOfCubes = nX * nY * nZ;
	////m_texturedCubes = new TextureObject[m_nrOfCubes];
	//
	//float distance = 4;
	//
	//Vector3f startPos = Vector3f((nX - 1) * -0.5f, 1.0f, (nZ - 1) * -0.5f) * distance;
	//
	//for (unsigned int z = 0; z < nZ; z++)
	//{
	//	for (unsigned int y = 0; y < nY; y++)
	//	{
	//		for (unsigned int x = 0; x < nX; x++)
	//		{
	//			//int index = x + y * nX + z * nX * nY;
	//
	//			//if (!m_texturedCubes[index].Initialize("cube_uv.obj", "Torgue.png"))
	//			/*if (!m_texturedCubes[index].Initialize("turret.obj", "turret_tex_v3.png"))
	//				return false;*/
	//
	//			m_scene->AddTexturedObject(
	//				"cube_uv.obj",
	//				"Torgue.png",
	//				startPos + Vector3f(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) * distance,
	//				Vector3f(0, 0, 0),
	//				Vector3f(1, 1, 1)
	//			);
	//
	//			//m_texturedCubes[index].SetPosition(startPos + Vector3f(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) * distance);
	//
	//			/* --------------------------------------- Insertion into quadtree -------------------------------------- */
	//			/*if (!m_quadTree.get()->Insert(&m_texturedCubes[index]))
	//			return false;*/
	//		}
	//	}
	//}
	//
	// /* ------------------------------------------- Floor ---------------------------------------------- */
	//
	//m_coloredFloor = std::make_unique<SingleColorObject>();
	//if (!m_coloredFloor.get()->Initialize("cube.obj", Vector3f(1, 0, 0)))
	//	return false;
	//
	//m_coloredFloor.get()->SetScale(1000.0f, 0.02f, 1000.0f);
	//m_coloredFloor.get()->SetPosition(0.0f, 0.0f, 0.0f);*/
	//
	//m_scene->AddSingleColoredObject(
	//	"cube.obj",
	//	Vector3f(1, 0, 0),
	//	Vector3f(0, 0, 0),
	//	Vector3f(0, 0, 0),
	//	Vector3f(1000.0f, 0.02f, 1000.0f)
	//);


	/* ============================================= Cameras ============================================= */
	Vector3f firstObjectPosition = m_scene->GetTexturedObjects()[0]->GetPosition();

	// Reset cameras
	PlayerCameraManager::Delete();

	cam = PlayerCameraManager::Get()->CreateCamera();
	cam->SetDimensions(Window::Get()->GetDimensions());
	cam->SetPosition(firstObjectPosition + Vector3f(0, 5, -5));
	cam->SetTarget(firstObjectPosition);
	cam->Update();

	/* ============================================= Player character ============================================== */
	m_player = std::make_unique<Character>();
	m_player.get()->SetMovementSpeed(5.0f);
	m_player.get()->SetPosition(PlayerCameraManager::Get()->GetCurrentCamera()->GetPosition());
	m_player.get()->SetLookDirection(PlayerCameraManager::Get()->GetCurrentCamera()->GetTargetDirection());


	/* ================================================ HUD ====================================================== */
	/*m_HUDTexture = std::make_unique<HUDTexture>();
	if (!m_HUDTexture.get()->Initialize(Direct3D::Get()->GetDevice(), "Torgue.png", Vector2i(0, 0), Vector2i(400, 400)))
		return false;
	m_HUDTexture.get()->SetPosition(Vector2i(500, 500));
	m_HUDTexture.get()->SetDimensions(Vector2i(200, 200));*/

	/* ============================================= Lights ====================================================== */
	PointLightManager::Delete();
	
	lightManager = PointLightManager::Get();
	lightManager->AddPointLight();

	PointLight* light = lightManager->GetPointLight(0);

	if (!light->Initialize(Direct3D::Get()->GetDevice(), Window::Get()->GetDimensions() * 4))
		return false;
	light->SetPosition(Vector3f(-40.0f, 20.0f, -20.0f));
	light->SetTarget(Vector3f(0.0f, 0.0f, 0.0f));
	light->Update();

	/* ============================================ FPS counter ==================================================== */
	m_fpsCounter = std::make_unique<FPSCounter>();
	m_fpsCounter->Initialize();

	/* ============================================ Modes and settings ============================================= */
	m_renderMode = RenderMode::NORMAL_MODE;
	m_HUDMode = HUDMode::HUD_OFF;
	m_orthogonal = OrthogonalMode::ORTHOGONAL_OFF;

	MapProjectionMatrix();
#endif

	return true;
}

void PlayState::ProcessInput()
{
	/*Input* input = Input::Get();
	PlayerCameraManager* manager = FrameWork::Get()->GetPlayerCameraManager();
	Character* player = m_player.get();

	//input->Update();

	if (input->IsKeyPressed(VK_ESCAPE))
	{
		m_stateMachine->Pop();
		//EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
		return;
		// Send event to stop game
	}

	player->SetIsMovingRight(input->IsKeyDown('D'));
	player->SetIsMovingLeft(input->IsKeyDown('A'));
	player->SetIsMovingForward(input->IsKeyDown('W'));
	player->SetIsMovingBackward(input->IsKeyDown('S'));
	player->SetIsMovingUp(input->IsKeyDown(VK_SPACE));
	player->SetIsMovingDown(input->IsKeyDown(VK_SHIFT));

	// Toggle boost
	if (input->IsKeyPressed('B'))
	{
		player->ToggleMovementBoost();
	}

	// Switch cameras
	if (input->IsKeyPressed(VK_RIGHT))
	{
		manager->ChangeUp();
		player->SetPosition(manager->GetCurrentCamera()->GetPosition());
		player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
	}
	if (input->IsKeyPressed(VK_LEFT))
	{
		manager->ChangeDown();
		player->SetPosition(manager->GetCurrentCamera()->GetPosition());
		player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
	}

	// Create camera as a copy of the current one
	if (input->IsKeyPressed(VK_RETURN))
	{
		manager->CreateCamera();
	}

	// Look at origin
	if (input->IsKeyDown('R'))
	{
		manager->GetCurrentCamera()->SetTarget(0.0f, 0.0f, 0.0f);
	}

	// Change cube texture and turn floor green
	if (input->IsKeyPressed('T'))
	{
		static bool toggle = false;
		toggle = !toggle;

		TextureObject* obj = m_scene->GetTexturedObjects()[0];

		if (toggle)
		{
			obj->SetTextureName("BrickWallRaw.jpg");
			//m_coloredFloor.get()->SetColor(0.0f, 1.0f, 0.0f);
		}
		else
		{
			obj->SetTextureName("Torgue.png");
			//m_coloredFloor.get()->SetColor(1.0f, 0.0f, 0.0f);
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
			Engine::Get()->EnableDepthTests();
			break;
		case false:
			Engine::Get()->DisableDepthTests();
			break;
		}
	}

	// Toggle orthogonal projection
	if (input->IsKeyPressed('P'))
	{
		m_orthogonal = static_cast<OrthogonalMode>(1 - m_orthogonal);

		MapProjectionMatrix();
	}

	// Change render mode
	if (input->IsKeyPressed(49))	// 1
	{
		m_renderMode = static_cast<RenderMode>((m_renderMode + 1) % RenderMode::NR_OF_RENDER_MODES);
	}

	// Change hud mode
	if (input->IsKeyPressed(50))	// 2
	{
		m_HUDMode = (HUDMode)((m_HUDMode + 1) % HUDMode::NR_OF_HUD_MODES);

		switch (m_HUDMode)
		{
		case TORGUE_HUD:
			//m_HUDTexture->SetDefaultShaderResourceView();
			break;
		default:
			break;
		}
	}

	// Change number of lights
	if (input->IsKeyPressed(VK_UP))
	{
		/*if (PointLightManager::Get()->AddPointLight())
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
			//std::vector<TextureObject*> objects = m_scene->GetTexturedObjects();
			TextureObject** objects = m_scene->GetTexturedObjects();
			unsigned int nrOfCubes = m_scene->GetNrOfTexturedObjects();

			for (unsigned int i = 0; i < nrOfCubes; i++)
			{
				//m_texturedCubes[i].Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
				//objects[i]->Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
			}
		}
	}
	else
	{
		mouseMovement *= 0.015f;

		manager->GetCurrentCamera()->RotateUp(mouseMovement.y);
		manager->GetCurrentCamera()->RotateRight(mouseMovement.x);
		player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
	}*/
}

void PlayState::Update(float dt)
{
#ifdef HYBRID_MODE
	m_fpsCounter.get()->Update(dt);
#elif defined OLD_MODE
	static float angle = 0.0f;
	unsigned int n;

	PointLightManager* lightManager = PointLightManager::Get();
	PlayerCameraManager* cameraManager = PlayerCameraManager::Get();
	Camera* cam = cameraManager->GetCurrentCamera();

	angle += dt;

	m_player.get()->Update(dt);

	cam->SetPosition(m_player.get()->GetPosition());
	cam->SetTarget(m_player.get()->GetPosition() + m_player.get()->GetLookDirection());
	cameraManager->Update();

	n = lightManager->GetNrOfPointLights();
	for (unsigned int i = 0; i < n; i++)
	{
		lightManager->GetPointLight(i)->SetPosition(Vector3f(std::cosf(angle + i * 0.5f), 1.0f, std::sinf(angle + i * 0.5f)) * 100.0f);
		lightManager->GetPointLight(i)->SetTarget(Vector3f(0, 0, 0));
		lightManager->GetPointLight(i)->Update();
	}

	m_scene->Update(dt);


	//CollisionWithScene();


	/*TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	n = m_scene->GetNrOfTexturedObjects();

	for (unsigned int i = 0; i < n; i++)
	{
		//m_texturedCubes[i].Rotate(0, dt, 0);
		//m_texturedCubes[i].Update();
		texturedObjects[i]->Update();
	}

	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();
	n = m_scene->GetNrOfSingleColoredObjects();

	for (unsigned int i = 0; i < n; i++)
	{
		//m_texturedCubes[i].Rotate(0, dt, 0);
		//m_texturedCubes[i].Update();
		singleColoredObjects[i]->Update();
	}*/

	//m_coloredFloor.get()->Update();

	m_fpsCounter.get()->Update(dt);

	/*AABA aaba;
	aaba.center = Vector2f(0, 0);
	aaba.halfSides = Vector2f(1, 1);
	std::list<QuadTreeObject*> objs;
	m_quadTree->RetrieveObjects(aaba, objs);*/

	//CubeIntersection();

	ConstantBufferStorage::Get()->SetVSViewMatrix(Direct3D::Get()->GetDeviceContext(), cam->GetViewMatrix());
#endif
}

void PlayState::MapProjectionMatrix()
{
	/*Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	ConstantBufferStorage* storage = ConstantBufferStorage::Get();
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	storage->SetVSProjectionMatrix(deviceContext, m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());*/
}

void PlayState::CubeIntersection()
{
	/*Camera* cam = FrameWork::Get()->GetPlayerCameraManager()->GetCurrentCamera();
	Vector3f corners[8];

	// The 8 corners of frustum in world coordinates
	FrustumCorners(cam->GetViewMatrix(), cam->GetProjectionMatrix(), corners);

	// Mouse coordinates on screen, in range [(0, 0), (WNDW, WNDH)]
	Vector2i mousePos = Input::Get()->MousePosition();

	// Mouse coordinates in percent, in range [(0, 0), (1, 1)]
	Vector2f mousePercentage = mousePos;
	Vector2i windowSize = Engine::Get()->GetWindowSize();
	mousePercentage.x /= windowSize.x;
	mousePercentage.y /= windowSize.y;
	mousePercentage.y = 1 - mousePercentage.y;

	Vector3f mouseDirectionOnFarPlaneX = (corners[5] - corners[1]) * mousePercentage.x;
	Vector3f mouseDirectionOnFarPlaneY = (corners[3] - corners[1]) * mousePercentage.y;
	Vector3f mouseDirectionOnNearPlaneX = (corners[4] - corners[0]) * mousePercentage.x;
	Vector3f mouseDirectionOnNearPlaneY = (corners[2] - corners[0]) * mousePercentage.y;

	Vector3f mousePositionOnFarPlane = (corners[1] + mouseDirectionOnFarPlaneX + mouseDirectionOnFarPlaneY);
	Vector3f mousePositionOnNearPlane = (corners[0] + mouseDirectionOnNearPlaneX + mouseDirectionOnNearPlaneY);

	Vector3f direction = (mousePositionOnFarPlane - mousePositionOnNearPlane).normalized();
	Vector3f origin = mousePositionOnNearPlane;

	OBB obb;
	obb.halfSides[0] = 1;
	obb.halfSides[1] = 1;
	obb.halfSides[2] = 1;
	obb.vectors[0] = Vector3f(1, 0, 0);
	obb.vectors[1] = Vector3f(0, 1, 0);
	obb.vectors[2] = Vector3f(0, 0, 1);

	Sphere sphere;
	sphere.radius2 = obb.halfSides[0] * obb.halfSides[0], obb.halfSides[1] * obb.halfSides[1], obb.halfSides[2] * obb.halfSides[2];*/


	/*TextureObject** objects = m_scene->GetTexturedObjects();
	unsigned int n = m_scene->GetNrOfTexturedObjects();

	for (unsigned int i = 0; i < n; i++)
	{
		IntersectionData data;

		sphere.center = objects[i]->GetPosition();
		data = RayVSSphere(origin, direction, sphere);

		if (data.intersection)
		{
			obb.center = sphere.center;
			data = RayVsOBB(origin, direction, obb);

			if (data.intersection)
			{
				if (objects[i]->GetTextureName() != "BrickWallRaw.jpg")
				{
					RenderManager::Get()->ChangeTexture(objects[i], "BrickWallRaw.jpg");
					//object->SetTextureName("BrickWallRaw.jpg");
				}
				continue;
			}
		}

		if (objects[i]->GetTextureName() != "Torgue.png")
		{
			RenderManager::Get()->ChangeTexture(objects[i], "Torgue.png");
			//object->SetDefaultTexture();
		}
	}*/

	//m_HUDTexture.get()->SetPosition(mousePos);
}

//void PlayState::CollisionWithScene()
//{
//	OBB playerOBB;
//	playerOBB.center = m_player->GetPosition() - Vector3f(0, 1, 0);
//	playerOBB.halfSides[0] = playerOBB.halfSides[2] = 0.5f;
//	playerOBB.halfSides[1] = 1.0f;
//
//	unsigned int nrOfObjects = m_scene->GetNrOfTexturedObjects();
//	TextureObject** objects = m_scene->GetTexturedObjects();
//
//	Collision* collision = Collision::Get();
//
//	for (unsigned int i = 0; i < nrOfObjects; i++)
//	{
//		// Move player away from object
//		while (collision->OBBVSOBB(playerOBB, objects[i]->GetOBB()))
//		{
//			Vector3f toPlayer = m_player->GetPosition() - objects[i]->GetPosition();
//			m_player->Move(toPlayer.normalized() * 0.01f);
//			playerOBB.center = m_player->GetPosition();
//		}
//	}
//}

void PlayState::Render()
{
#ifdef HYBRID_MODE
	RenderHUDText();
#elif defined OLD_MODE
	/*Direct3D* d3d = Direct3D::Get();
	d3d->ClearDefaultTarget();*/

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

	//switch (m_HUDMode)
	//{
	//case TORGUE_HUD:
	//	// The shader resource view is set when this mode is set
	//	//RenderHUD();
	//	break;
	//case DEFERRED_POSITIONS:
	//	//m_HUDTexture.get()->SetShaderResourceView(d3d->GetDeferredShaderResourceViews()[0]);
	//	//RenderHUD();
	//	break;
	//case DEFERRED_NORMALS:
	//	//m_HUDTexture.get()->SetShaderResourceView(d3d->GetDeferredShaderResourceViews()[1]);
	//	//RenderHUD();
	//	break;
	//case DEFERRED_COLORS:
	//	//m_HUDTexture.get()->SetShaderResourceView(d3d->GetDeferredShaderResourceViews()[2]);
	//	//RenderHUD();
	//	break;
	//default:
	//	break;
	//}

	RenderHUDText();

	//d3d->Present();
#endif
}

void PlayState::RenderNormal()
{
	/*ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	bufferStorage->SetVSPointLight(deviceContext, cam0->GetPosition(), 1.0f);
	RenderManager::Get()->RenderTexturedObjects();

	bufferStorage->SetPSPointLight(deviceContext, cam0->GetPosition(), 1.0f);
	RenderManager::Get()->RenderSingleColoredObjects();*/
}
void PlayState::RenderDeferredFirstPass()
{
	/*Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();

	ModelStorage* modelStorage = ModelStorage::Get();
	TextureStorage* textureStorage = TextureStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;
	ID3D11ShaderResourceView* texture;

	TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();

	unsigned int n;

	// ============================= Quadtree retrieval =============================
	//Frustum frustum;
	//AABB frustumAABB;
	//std::list<QuadTreeObject*> objects;
	//
	////frustum.Create(cam->GetViewMatrix(), cam->GetProjectionMatrix());
	////frustumAABB = FrustumAABB(cam->GetViewMatrix(), cam->GetProjectionMatrix());
	//frustumAABB.center = Vector3f(0, 10, 0);
	//frustumAABB.halfSides = Vector3f(200, 10, 200);
	//m_quadTree.get()->RetrieveObjects(frustumAABB, objects);


	d3d->SetDeferredTargets();
	d3d->ClearDeferredTargets();

	bufferStorage->SetVSViewMatrix(deviceContext, cam->GetViewMatrix());
	bufferStorage->SetVSProjectionMatrix(deviceContext, m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	// ========================= Render texture objects ==========================
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);

	// ------------------------- Render cubes -------------------------
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].get()->GetModelName());
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0]->GetModelName());
	//textureModel = modelStorage->GetTextureModel(m_texturedCubes[0].GetModelName());
	textureModel = modelStorage->GetTextureModel(texturedObjects[0]->GetModelName());
	textureModel->SetupRender();

	//texture = textureStorage->GetTexture(m_texturedCubes[0].get()->GetTextureName());
	//texture = textureStorage->GetTexture(m_texturedCubes[0]->GetTextureName());
	//texture = textureStorage->GetTexture(m_texturedCubes[0].GetTextureName());
	texture = textureStorage->GetTexture(texturedObjects[0]->GetTextureName());
	shaders->SetPerObjectDeferredTextureConstantBuffer(deviceContext, texture);

	//std::list<QuadTreeObject*>::iterator it;
	//for (it = objects.begin(); it != objects.end(); it++)
	//{
	//	TextureObject* obj = dynamic_cast<TextureObject*>(*it);
	//	bufferStorage->SetVSWorldMatrix(deviceContext, obj->GetWorldMatrix());
	//	textureModel->Render(deviceContext);
	//}

	n = m_scene->GetNrOfTexturedObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i].get()->GetWorldMatrix());
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i]->GetWorldMatrix());
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_texturedCubes[i].GetWorldMatrix());
		bufferStorage->SetVSWorldMatrix(deviceContext, texturedObjects[i]->GetWorldMatrix());

		//shaders->SetPerObjectDeferredTextureConstantBuffer(deviceContext, textureStorage->GetTexture(m_texturedCubes[i].GetTextureName()));

		textureModel->Render();
	}


	// ========================= Render single color objects ==========================
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);

	// ------------------------- Render floor -------------------------
	singleColorModel = modelStorage->GetSingleColorModel(singleColoredObjects[0]->GetModelName());
	singleColorModel->SetupRender();

	bufferStorage->SetVSColor(deviceContext, singleColoredObjects[0]->GetColor());

	n = m_scene->GetNrOfSingleColoredObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		//bufferStorage->SetVSWorldMatrix(deviceContext, m_coloredFloor.get()->GetWorldMatrix());
		bufferStorage->SetVSWorldMatrix(deviceContext, singleColoredObjects[i]->GetWorldMatrix());

		singleColorModel->Render();
	}*/
}
void PlayState::RenderDeferredFirstPassChunks()
{
	/*Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();

	ModelStorage* modelStorage = ModelStorage::Get();
	TextureStorage* textureStorage = TextureStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;
	ID3D11ShaderResourceView* texture;

	TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();

	d3d->SetDeferredTargets();
	d3d->ClearDeferredTargets();

	bufferStorage->SetGSViewMatrix(deviceContext, cam->GetViewMatrix());
	bufferStorage->SetGSProjectionMatrix(deviceContext, m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	// ========================= Render texture objects ==========================
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE_CHUNK);

	// ------------------------- Render cubes -------------------------
	textureModel = modelStorage->GetTextureModel(texturedObjects[0]->GetModelName());
	textureModel->SetupRender();

	texture = textureStorage->GetTexture(texturedObjects[0]->GetTextureName());
	shaders->SetPerObjectDeferredTextureChunkShaderGroup(deviceContext, texture);

	DirectX::XMMATRIX worldMatrices[CHUNK_SIZE];

	unsigned int nrOfFullRenderPasses = m_scene->GetNrOfTexturedObjects() / CHUNK_SIZE;
	unsigned int nrOfCubesInFullRenderPasses = nrOfFullRenderPasses * CHUNK_SIZE;
	unsigned int nrOfRemainingCubes = m_scene->GetNrOfTexturedObjects() - nrOfCubesInFullRenderPasses;

	// Render the full chunks of objects
	for (unsigned int i = 0; i < nrOfFullRenderPasses; i++)
	{
		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
		{
			DirectX::XMMATRIX temp = texturedObjects[i * CHUNK_SIZE + j]->GetWorldMatrix();
			memcpy(&worldMatrices[j], &temp, sizeof(temp));
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, CHUNK_SIZE);
		textureModel->Render();
	}

	// Render the leftovers
	if (nrOfRemainingCubes > 0)
	{
		for (unsigned int i = 0; i < nrOfRemainingCubes; i++)
		{
			DirectX::XMMATRIX temp = texturedObjects[nrOfCubesInFullRenderPasses + i]->GetWorldMatrix();
			memcpy(&worldMatrices[i], &temp, sizeof(temp));
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, nrOfRemainingCubes);
		textureModel->Render();
	}


	// ========================= Render single color objects ==========================
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);

	// ------------------------- Render floor -------------------------
	singleColorModel = modelStorage->GetSingleColorModel(singleColoredObjects[0]->GetModelName());
	singleColorModel->SetupRender();

	bufferStorage->SetVSColor(deviceContext, singleColoredObjects[0]->GetColor());

	unsigned int n = m_scene->GetNrOfSingleColoredObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		bufferStorage->SetVSWorldMatrix(deviceContext, singleColoredObjects[i]->GetWorldMatrix());
		singleColorModel->Render();
	}*/
}

void PlayState::RenderDepth()
{
	/*Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();

	ModelStorage* modelStorage = ModelStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();

	unsigned int n;

	shaders->SetPerFrameDepthConstantBuffer(
		deviceContext,
		cam->GetViewMatrix(),
		m_orthogonal ? cam->GetOrthogonalMatrix() : cam->GetProjectionMatrix());

	// ========================= Render texture objects ==========================
	shaders->SetShaderType(deviceContext, ShaderType::DEPTH);

	// ------------------------- Render cubes -------------------------
	textureModel = modelStorage->GetTextureModel(texturedObjects[0]->GetModelName());
	textureModel->SetupRender();

	n = m_scene->GetNrOfTexturedObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		shaders->SetPerObjectDepthConstantBuffer(deviceContext, texturedObjects[i]->GetWorldMatrix());
		textureModel->Render();
	}

	// ========================= Render single color objects ==========================
	// ------------------------- Render floor -------------------------
	singleColorModel = modelStorage->GetSingleColorModel(singleColoredObjects[0]->GetModelName());
	singleColorModel->SetupRender();

	n = m_scene->GetNrOfSingleColoredObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		shaders->SetPerObjectDepthConstantBuffer(deviceContext, singleColoredObjects[i]->GetWorldMatrix());
		singleColorModel->Render();
	}*/
}
void PlayState::RenderShadowPass()
{
	/*Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();


	unsigned int n;

	d3d->SetShadowTarget();
	d3d->ClearShadowTarget();

	bufferStorage->SetVSViewMatrix(deviceContext, cam0->GetViewMatrix());
	bufferStorage->SetVSProjectionMatrix(deviceContext, m_orthogonal ? cam0->GetOrthogonalMatrix() : cam0->GetProjectionMatrix());

	// ========================= Render texture objects ==========================
	// Set texture shaders, then remove pixel shader
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	// ------------------------- Render cubes -------------------------
	textureModel = modelStorage->GetTextureModel(texturedObjects[0]->GetModelName());
	textureModel->SetupRender();

	n = m_scene->GetNrOfTexturedObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		bufferStorage->SetVSWorldMatrix(deviceContext, texturedObjects[i]->GetWorldMatrix());
		textureModel->Render();
	}

	// ========================= Render single color objects ==========================
	// Set color shaders, then remove pixel shader
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	// ------------------------- Render floor -------------------------
	singleColorModel = modelStorage->GetSingleColorModel(singleColoredObjects[0]->GetModelName());
	singleColorModel->SetupRender();

	n = m_scene->GetNrOfSingleColoredObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		bufferStorage->SetVSWorldMatrix(deviceContext, singleColoredObjects[i]->GetWorldMatrix());
		singleColorModel->Render();
	}*/
}
void PlayState::RenderShadowPassChunks()
{
	/*Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);

	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();

	d3d->SetShadowTarget();
	d3d->ClearShadowTarget();

	bufferStorage->SetGSViewMatrix(deviceContext, cam0->GetViewMatrix());
	bufferStorage->SetGSProjectionMatrix(deviceContext, m_orthogonal ? cam0->GetOrthogonalMatrix() : cam0->GetProjectionMatrix());

	// ========================= Render texture objects ==========================
	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE_CHUNK);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	// ------------------------- Render cubes -------------------------
	textureModel = modelStorage->GetTextureModel(texturedObjects[0]->GetModelName());
	textureModel->SetupRender();

	DirectX::XMMATRIX worldMatrices[CHUNK_SIZE];

	unsigned int nrOfFullRenderPasses = m_scene->GetNrOfTexturedObjects() / CHUNK_SIZE;
	unsigned int nrOfCubesInFullRenderPasses = nrOfFullRenderPasses * CHUNK_SIZE;
	unsigned int nrOfRemainingCubes = m_scene->GetNrOfTexturedObjects() - nrOfCubesInFullRenderPasses;

	// Render the full chunks of objects
	for (unsigned int i = 0; i < nrOfFullRenderPasses; i++)
	{
		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
		{
			DirectX::XMMATRIX temp = texturedObjects[i * CHUNK_SIZE + j]->GetWorldMatrix();
			memcpy(&worldMatrices[j], &temp, sizeof(temp));
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, CHUNK_SIZE);
		textureModel->Render();
	}

	// Render the leftovers
	if (nrOfRemainingCubes > 0)
	{
		for (unsigned int i = 0; i < nrOfRemainingCubes; i++)
		{
			DirectX::XMMATRIX temp = texturedObjects[nrOfCubesInFullRenderPasses + i]->GetWorldMatrix();
			memcpy(&worldMatrices[i], &temp, sizeof(temp));
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, nrOfRemainingCubes);
		textureModel->Render();
	}

	// ========================= Render single color objects ==========================
	// Set color shaders, then remove pixel shader
	shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

	// ------------------------- Render floor -------------------------
	singleColorModel = modelStorage->GetSingleColorModel(singleColoredObjects[0]->GetModelName());
	singleColorModel->SetupRender();

	unsigned int n = m_scene->GetNrOfSingleColoredObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		bufferStorage->SetVSWorldMatrix(deviceContext, singleColoredObjects[i]->GetWorldMatrix());
		singleColorModel->Render();
	}*/
}
void PlayState::RenderMultipleShadowsPass()
{
	/*Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();

	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	PointLightManager* lightManager = PointLightManager::Get();

	TextureModel* textureModel;
	SingleColorModel* singleColorModel;

	TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();

	PointLight* pointLight;

	unsigned int n;

	for (unsigned int i = 0; i < lightManager->GetNrOfPointLights(); i++)
	{
		pointLight = lightManager->GetPointLight(i);

		pointLight->Set(deviceContext);
		pointLight->Clear(deviceContext);

		bufferStorage->SetVSViewMatrix(deviceContext, pointLight->GetView());
		bufferStorage->SetVSProjectionMatrix(deviceContext, pointLight->GetProjection());

		// ========================= Render texture objects ========================== 
		// Set texture shaders, then remove pixel shader
		shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE);
		shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

		// ------------------------- Render cubes -------------------------
		textureModel = modelStorage->GetTextureModel(texturedObjects[i]->GetModelName());
		textureModel->SetupRender();

		n = m_scene->GetNrOfTexturedObjects();
		for (unsigned int j = 0; j < n; j++)
		{
			bufferStorage->SetVSWorldMatrix(deviceContext, texturedObjects[j]->GetWorldMatrix());
			textureModel->Render();
		}

		// ========================= Render single color objects ==========================
		// Set color shaders, then remove pixel shader
		shaders->SetShaderType(deviceContext, ShaderType::D_SINGLE_COLOR);
		shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);

		// ------------------------- Render floor -------------------------
		singleColorModel = modelStorage->GetSingleColorModel(singleColoredObjects[0]->GetModelName());
		singleColorModel->SetupRender();

		n = m_scene->GetNrOfSingleColoredObjects();
		for (unsigned int i = 0; i < n; i++)
		{
			bufferStorage->SetVSWorldMatrix(deviceContext, singleColoredObjects[i]->GetWorldMatrix());
			singleColorModel->Render();
		}
	}*/
}

void PlayState::RenderDeferredLightShadowPass()
{
	/*Direct3D* d3d = Direct3D::Get();
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

	DeferredScreenTarget::Get()->Render(deviceContext);*/
}
void PlayState::RenderDeferredLightSplitPass()
{
	/*Direct3D* d3d = Direct3D::Get();
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

	DeferredScreenTarget::Get()->Render(deviceContext);*/
}
void PlayState::RenderDeferredLightMultiplePass()
{
	/*Direct3D* d3d = Direct3D::Get();
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

	DeferredScreenTarget::Get()->Render(deviceContext);*/
}
void PlayState::RenderDeferredLightMultipleShadowsPass()
{
	/*Direct3D* d3d = Direct3D::Get();
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

	DeferredScreenTarget::Get()->Render(deviceContext);*/
}
//void PlayState::RenderHUD()
//{
//	Direct3D* d3d = Direct3D::Get();
//	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
//	ShaderManager* shaders = ShaderManager::Get();
//
//	shaders->SetShaderType(deviceContext, ShaderType::HUD);
//
//	shaders->SetPerObjectHUDConstantBuffer(
//		deviceContext,
//		m_HUDTexture->GetShaderResourceView());
//
//	// Disable depth test, so everything below is rendered above everything else
//	d3d->DisableZBuffer();
//
//	m_HUDTexture.get()->Render(deviceContext);
//
//	// Reenable
//	d3d->EnableZBuffer();
//}
void PlayState::RenderHUDText()
{
	/*Direct3D* d3d = Direct3D::Get();

	d3d->DisableZBuffer();

	m_fpsCounter.get()->Render();

	d3d->EnableZBuffer();
	d3d->SetDefaultBlendState();*/
}