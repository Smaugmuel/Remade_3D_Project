#include "SceneEditorState.hpp"

// For this game state
#include "StateMachine.hpp"

// For each editor state
#include "EditorSelectionState.hpp"
#include "EditorMoveState.hpp"
#include "EditorRotateState.hpp"
#include "EditorScaleState.hpp"
#include "EditorPlacementState.hpp"
#include "EditorSaveState.hpp"
#include "EditorLoadState.hpp"

#include "Window.hpp"
#include "Direct3D.hpp"
#include "Input.hpp"

#include "WindowSettings.hpp"

#include "ShaderManager.hpp"
#include "ShaderStorage.hpp"
#include "ConstantBufferStorage.hpp"
#include "ModelStorage.hpp"
#include "TextureStorage.hpp"
#include "SceneStorage.hpp"

#include "EventDispatcher.hpp"

#include "FPS_Counter.hpp"

#include "Scene.hpp"

#include "PointLightManager.hpp"

// For cameras and movements
#include "Camera.hpp"
#include "PlayerCameraManager.hpp"
#include "Character.hpp"

#include "RenderManager.hpp"

//#include "World.hpp"


#include "LineBeam.hpp"

SceneEditorState::SceneEditorState(StateMachine<GameState>* stateMachine) : GameState::GameState(stateMachine), m_editorMode(EditorModes::SELECT)
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(EditorModes::NR_OF_EDITOR_MODES); i++)
	{
		m_editorStates[i] = nullptr;
	}

	EventDispatcher::Get()->Subscribe(EventType::SWITCHED_SCENE, this);
}

SceneEditorState::~SceneEditorState()
{
	RenderManager::Delete();

	for (unsigned int i = 0; i < static_cast<unsigned int>(EditorModes::NR_OF_EDITOR_MODES); i++)
	{
		delete m_editorStates[i];
	}

	delete m_beam;

	EventDispatcher::Get()->Unsubscribe(EventType::SWITCHED_SCENE, this);
}

bool SceneEditorState::Initialize()
{
	/* ======================================= Editor states ====================================== */
	m_editorMode = EditorModes::SELECT;
	
	m_editorStates[static_cast<unsigned int>(EditorModes::SELECT)] = new EditorSelectionState;
	m_editorStates[static_cast<unsigned int>(EditorModes::MOVE)] = new EditorMoveState;
	m_editorStates[static_cast<unsigned int>(EditorModes::ROTATE)] = new EditorRotateState;
	m_editorStates[static_cast<unsigned int>(EditorModes::SCALE)] = new EditorScaleState;
	m_editorStates[static_cast<unsigned int>(EditorModes::PLACE)] = new EditorPlacementState;
	m_editorStates[static_cast<unsigned int>(EditorModes::SAVE)] = new EditorSaveState;
	m_editorStates[static_cast<unsigned int>(EditorModes::LOAD)] = new EditorLoadState;

	for (unsigned int i = 0; i < static_cast<unsigned int>(EditorModes::NR_OF_EDITOR_MODES); i++)
	{
		if (!m_editorStates[i]->Initialize())
		{
			return false;
		}
	}

	// Initialize the world
	//World::Get();

	/* ============================================= Initial scene ======================================= */
	if (!SceneStorage::Get()->LoadScene("Scene4"))
		return false;
	
	m_scene = SceneStorage::Get()->GetScene("Scene4");
	m_scene->LoadIntoRenderManager();
	
	EventDispatcher::Get()->Emit(Event(EventType::SWITCHED_SCENE, static_cast<void*>(m_scene)));
	
	/* ============================================= Cameras ============================================= */
	unsigned int nrOfObjects = m_scene->GetNrOfTexturedObjects();
	unsigned int nrOfObjectsPerSide = static_cast<unsigned int>(sqrt(nrOfObjects));
	
	Vector3f startPos = Vector3f((nrOfObjectsPerSide - 1) * -0.5f, 1.0f, (nrOfObjectsPerSide - 1) * -0.5f) * 4;

	PlayerCameraManager::Delete();
	Camera* cam = PlayerCameraManager::Get()->CreateCamera();
	cam->SetDimensions(Window::Get()->GetDimensions());
	cam->SetPosition(startPos + Vector3f(0, 5, -5));
	cam->SetTarget(startPos);
	cam->Update();
	
	/* ============================================= Player character ============================================== */
	m_player = std::make_unique<Character>();
	m_player.get()->SetMovementSpeed(5.0f);
	m_player.get()->SetPosition(PlayerCameraManager::Get()->GetCurrentCamera()->GetPosition());
	m_player.get()->SetLookDirection(PlayerCameraManager::Get()->GetCurrentCamera()->GetTargetDirection());
	
	/* ============================================ FPS counter ==================================================== */
	m_fpsCounter = std::make_unique<FPSCounter>();
	m_fpsCounter.get()->Initialize(Direct3D::Get()->GetDevice(), Direct3D::Get()->GetDeviceContext());

	// Laser beam
	m_beam = new LineBeam;
	if (!m_beam->Initialize(Vector3f(-25, 5, -20)))
	{
		return false;
	}

	MapProjectionMatrix();

	return true;
}

void SceneEditorState::ProcessInput()
{
	Input* input = Input::Get();
	PlayerCameraManager* manager = PlayerCameraManager::Get();
	Character* player = /*World::Get()->GetPlayer();*/m_player.get();
	Vector2f mouseMovement;

	//input->Update();

	mouseMovement = input->MouseMovement();

	if (input->IsKeyPressed(VK_ESCAPE))
	{
		EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
		return;
		// Send event to leave editor
	}

	// Move player
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

	// Perform these actions only when CTRL is not held down
	if (!input->IsKeyDown(VK_CONTROL))
	{
		// Switch editor mode
		for (unsigned int i = 0; i < static_cast<unsigned int>(EditorModes::NR_OF_EDITOR_MODES); i++)
		{
			if (input->IsKeyPressed(49 + i))
			{
				m_editorMode = static_cast<EditorModes>(i);
				break;
			}
		}

		// Look at origin
		if (input->IsKeyDown('R'))
		{
			manager->GetCurrentCamera()->SetTarget(0.0f, 0.0f, 0.0f);
			player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
		}
	}

	// Rotate camera if mouse wheel is down
	if (input->IsKeyDown(VK_MBUTTON))
	{
		if (mouseMovement != Vector2f(0, 0))
		{
			mouseMovement *= 0.015f;

			manager->GetCurrentCamera()->RotateUp(mouseMovement.y);
			manager->GetCurrentCamera()->RotateRight(mouseMovement.x);
			player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
		}
	}

	m_editorStates[static_cast<unsigned int>(m_editorMode)]->ProcessInput();
}

void SceneEditorState::Update(float dt)
{
	static float angle = 0.0f;
	unsigned int n;

	PointLightManager* lightManager = PointLightManager::Get();
	PlayerCameraManager* cameraManager = PlayerCameraManager::Get();
	Camera* cam = cameraManager->GetCurrentCamera();

	Character* player = /*World::Get()->GetPlayer();*/m_player.get();

	angle += dt;

	player->Update(dt);
	//player->Update(dt);

	cam->SetPosition(player->GetPosition());
	cam->SetTarget(player->GetPosition() + player->GetLookDirection());
	cameraManager->Update();

	n = lightManager->GetNrOfPointLights();
	for (unsigned int i = 0; i < n; i++)
	{
		lightManager->GetPointLight(i)->SetPosition(Vector3f(std::cosf(angle + i * 0.5f), 1.0f, std::sinf(angle + i * 0.5f)) * 100.0f);
		lightManager->GetPointLight(i)->SetTarget(Vector3f(0, 0, 0));
		lightManager->GetPointLight(i)->Update();
	}

	m_beam->Update(dt);

	//World::Get()->GetScene()->Update(dt);
	m_scene->Update(dt);

	//m_editorStateMachine->Peek()->Update(dt);
	m_editorStates[static_cast<unsigned int>(m_editorMode)]->Update(dt);

	//World::Get()->GetFPSCounter()->Update(dt);
	m_fpsCounter.get()->Update(dt);

	ConstantBufferStorage::Get()->SetVSViewMatrix(Direct3D::Get()->GetDeviceContext(), cam->GetViewMatrix());
}

void SceneEditorState::MapProjectionMatrix()
{
	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
	ConstantBufferStorage* storage = ConstantBufferStorage::Get();
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	storage->SetVSProjectionMatrix(deviceContext, cam->GetProjectionMatrix());
}

void SceneEditorState::Render()
{
	Direct3D* d3d = Direct3D::Get();

	d3d->ClearDefaultTarget();

	RenderNormal();

	RenderHUD();

	d3d->Present();
}

void SceneEditorState::RenderNormal()
{
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	// Render textured objects
	bufferStorage->SetVSPointLight(deviceContext, cam0->GetPosition(), 1.0f);
	RenderManager::Get()->RenderTexturedObjects();

	// For some unknown reason, this can't happen after single colored objects are rendered
	m_editorStates[static_cast<unsigned int>(m_editorMode)]->Render();

	// Render single colored objects
	bufferStorage->SetPSPointLight(deviceContext, cam0->GetPosition(), 1.0f);
	RenderManager::Get()->RenderSingleColoredObjects();


	m_beam->Render();
}

void SceneEditorState::RenderHUD()
{
	Direct3D* d3d = Direct3D::Get();

	d3d->DisableZBuffer();

	m_editorStates[static_cast<unsigned int>(m_editorMode)]->RenderHUD();


	//World::Get()->GetFPSCounter()->Render();
	m_fpsCounter.get()->Render();

	d3d->EnableZBuffer();
	d3d->SetDefaultBlendState();
}

void SceneEditorState::ReceiveEvent(const Event & e)
{
	switch (e.type)
	{
	case EventType::SWITCHED_SCENE:
		//World::Get()->SetScene(static_cast<Scene*>(e.value));
		m_scene = static_cast<Scene*>(e.value);
		break;
	default:
		break;
	}
}
