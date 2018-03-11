#include "SceneEditorState.hpp"

#include "Window.hpp"
#include "Direct3D.hpp"
#include "Input.hpp"

#include "ShaderManager.hpp"
#include "ShaderStorage.hpp"
#include "ConstantBufferStorage.hpp"
#include "ModelStorage.hpp"
#include "TextureStorage.hpp"

#include "WindowSettings.hpp"

#include "EventDispatcher.hpp"

#include "FPS_Counter.hpp"

#include "SceneStorage.hpp"
#include "Scene.hpp"

#include "PointLightManager.hpp"

#include "Camera.hpp"
#include "PlayerCameraManager.hpp"

#include "Character.hpp"

#include "TextureObject.hpp"
#include "SingleColorObject.hpp"

#include "TextureModel.hpp"
#include "SingleColorModel.hpp"

#include "Collision.hpp"

SceneEditorState::SceneEditorState(StateMachine<GameState>* stateMachine) : GameState::GameState(stateMachine)
{
}

SceneEditorState::~SceneEditorState()
{
}

bool SceneEditorState::Initialize()
{
	m_scene = SceneStorage::Get()->GetScene("Scene3_1_turret");

	m_ghostObject = std::make_unique<TextureObject>();
	if (!m_ghostObject.get()->Initialize("../Models/turret.obj", "../Textures/turret_tex_v3.png"))
		return false;

	/* ============================================= Player character ============================================== */
	m_player = std::make_unique<Character>();
	m_player.get()->SetMovementSpeed(5.0f);
	m_player.get()->SetPosition(PlayerCameraManager::Get()->GetCurrentCamera()->GetPosition());
	m_player.get()->SetLookDirection(PlayerCameraManager::Get()->GetCurrentCamera()->GetTargetDirection());

	/* ============================================ FPS counter ==================================================== */
	m_fpsCounter = std::make_unique<FPSCounter>();
	m_fpsCounter.get()->Initialize(Direct3D::Get()->GetDevice(), Direct3D::Get()->GetDeviceContext());

	MapProjectionMatrix();

	return true;
}

void SceneEditorState::ProcessInput()
{
	Input* input = Input::Get();
	PlayerCameraManager* manager = PlayerCameraManager::Get();
	Character* player = m_player.get();
	Vector2f mouseMovement;

	input->Update();

	mouseMovement = input->MouseMovement();

	if (input->IsKeyPressed(VK_ESCAPE))
	{
		EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
		return;
		// Send event to leave editor
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

	// Load or save current scene
	if (input->IsKeyDown(VK_CONTROL))
	{
		if (input->IsKeyPressed('O'))
		{
			m_scene->SaveToFile(m_scene->GetName());
		}
		else if (input->IsKeyPressed('I'))
		{
			m_scene->LoadFromFile(m_scene->GetName());
		}
	}

	// Create an object at ghost object
	if (input->IsKeyPressed(VK_LBUTTON))
	{
		TextureObject* obj = m_ghostObject.get();
		m_scene->AddTexturedObject(obj->GetModelName(), obj->GetTextureName(), obj->GetPosition(), obj->GetRotation(), obj->GetScale());
	}

	// Rotate cubes or camera
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
				objects[i]->Rotate(mouseMovement.y * 0.01f * (i + 1), mouseMovement.x * 0.01f * (i + 1), 0.0f);
			}
		}
	}
	else
	{
		mouseMovement *= 0.015f;

		manager->GetCurrentCamera()->RotateUp(mouseMovement.y);
		manager->GetCurrentCamera()->RotateRight(mouseMovement.x);
		player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
	}
}

void SceneEditorState::Update(float dt)
{
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

	TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	n = m_scene->GetNrOfTexturedObjects();

	for (unsigned int i = 0; i < n; i++)
	{
		//m_texturedCubes[i].Rotate(0, dt, 0);
		//m_texturedCubes[i].Update();
		texturedObjects[i]->Update();
	}

	m_ghostObject.get()->Update();

	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();
	n = m_scene->GetNrOfSingleColoredObjects();

	for (unsigned int i = 0; i < n; i++)
	{
		//m_texturedCubes[i].Rotate(0, dt, 0);
		//m_texturedCubes[i].Update();
		singleColoredObjects[i]->Update();
	}

	//m_coloredFloor.get()->Update();

	m_fpsCounter.get()->Update(dt);

	CubeIntersection();

	ConstantBufferStorage::Get()->SetVSViewMatrix(Direct3D::Get()->GetDeviceContext(), cam->GetViewMatrix());
}

void SceneEditorState::MapProjectionMatrix()
{
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	ConstantBufferStorage* storage = ConstantBufferStorage::Get();
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	storage->SetVSProjectionMatrix(deviceContext, cam->GetProjectionMatrix());
}

void SceneEditorState::CubeIntersection()
{
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	Vector3f corners[8];

	// The 8 corners of frustum in world coordinates
	FrustumCorners(cam->GetViewMatrix(), cam->GetProjectionMatrix(), corners);

	// Mouse coordinates on screen, in range [(0, 0), (WNDW, WNDH)]
	Vector2i mousePos = Input::Get()->MousePosition();

	// Mouse coordinates in percent, in range [(0, 0), (1, 1)]
	Vector2f mousePercentage = mousePos;
	mousePercentage.x /= WNDW;
	mousePercentage.y /= WNDH;
	mousePercentage.y = 1 - mousePercentage.y;

	Vector3f mouseDirectionOnFarPlaneX = (corners[5] - corners[1]) * mousePercentage.x;
	Vector3f mouseDirectionOnFarPlaneY = (corners[3] - corners[1]) * mousePercentage.y;
	Vector3f mouseDirectionOnNearPlaneX = (corners[4] - corners[0]) * mousePercentage.x;
	Vector3f mouseDirectionOnNearPlaneY = (corners[2] - corners[0]) * mousePercentage.y;

	Vector3f mousePositionOnFarPlane = (corners[1] + mouseDirectionOnFarPlaneX + mouseDirectionOnFarPlaneY);
	Vector3f mousePositionOnNearPlane = (corners[0] + mouseDirectionOnNearPlaneX + mouseDirectionOnNearPlaneY);

	Vector3f direction = (mousePositionOnFarPlane - mousePositionOnNearPlane).normalized();
	Vector3f origin = mousePositionOnNearPlane;

	m_ghostObject.get()->SetPosition(origin + direction * 10);

	OBB obb;
	obb.halfSides[0] = 1;
	obb.halfSides[1] = 1;
	obb.halfSides[2] = 1;
	obb.vectors[0] = Vector3f(1, 0, 0);
	obb.vectors[1] = Vector3f(0, 1, 0);
	obb.vectors[2] = Vector3f(0, 0, 1);

	TextureObject* object = m_scene->GetTexturedObjects()[0];

	obb.center = object->GetPosition();

	IntersectionData data;
	data = RayVsOBB(origin, direction, obb);

	if (data.intersection)
	{
		object->SetTextureName("../Textures/BrickWallRaw.jpg");
	}
	else
	{
		object->SetDefaultTexture();
	}
}

void SceneEditorState::Render()
{
	Direct3D* d3d = Direct3D::Get();

	d3d->ClearDefaultTarget();

	RenderNormal();

	RenderHUDText();

	d3d->Present();
}

void SceneEditorState::RenderNormal()
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

	TextureObject** texturedObjects = m_scene->GetTexturedObjects();
	SingleColorObject** singleColoredObjects = m_scene->GetSingleColoredObjects();

	unsigned int n;

	/* ========================= Render texture objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::TEXTURE);
	bufferStorage->SetVSPointLight(deviceContext, cam0->GetPosition(), 1.0f);

	/* ------------------------- Render cubes ------------------------- */
	textureModel = modelStorage->GetTextureModel(texturedObjects[0]->GetModelName());
	textureModel->SetupRender(deviceContext);

	texture = textureStorage->GetTexture(texturedObjects[0]->GetTextureName());
	shaders->SetPerObjectTextureConstantBuffer(deviceContext, texture);

	n = m_scene->GetNrOfTexturedObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		bufferStorage->SetVSWorldMatrix(deviceContext, texturedObjects[i]->GetWorldMatrix());

		textureModel->Render(deviceContext);
	}

	bufferStorage->SetVSWorldMatrix(deviceContext, m_ghostObject.get()->GetWorldMatrix());
	textureModel->Render(deviceContext);


	/* ========================= Render single color objects ========================== */
	shaders->SetShaderType(deviceContext, ShaderType::SINGLE_COLOR);

	bufferStorage->SetPSPointLight(deviceContext, cam0->GetPosition(), 1.0f);

	/* ------------------------- Render floor ------------------------- */
	singleColorModel = modelStorage->GetSingleColorModel(singleColoredObjects[0]->GetModelName());

	n = m_scene->GetNrOfSingleColoredObjects();
	for (unsigned int i = 0; i < n; i++)
	{
		bufferStorage->SetVSWorldMatrix(deviceContext, singleColoredObjects[i]->GetWorldMatrix());
		bufferStorage->SetVSColor(deviceContext, singleColoredObjects[i]->GetColor());
	}

	singleColorModel->SetupRender(deviceContext);
	singleColorModel->Render(deviceContext);
}

void SceneEditorState::RenderHUDText()
{
	Direct3D* d3d = Direct3D::Get();

	d3d->DisableZBuffer();

	m_fpsCounter.get()->Render();

	d3d->EnableZBuffer();
	d3d->SetDefaultBlendState();
}