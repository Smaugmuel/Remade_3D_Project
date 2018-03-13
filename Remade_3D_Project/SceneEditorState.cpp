#include "SceneEditorState.hpp"

#include "StateMachine.hpp"
#include "EditorSelectionState.hpp"
#include "EditorMoveState.hpp"
#include "EditorPlacementState.hpp"

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

#include "Camera.hpp"
#include "PlayerCameraManager.hpp"
#include "Character.hpp"

#include "RenderManager.hpp"

SceneEditorState::SceneEditorState(StateMachine<GameState>* stateMachine) : GameState::GameState(stateMachine)
{
}

SceneEditorState::~SceneEditorState()
{
	RenderManager::Delete();

	delete m_editorStateMachine;
}

bool SceneEditorState::Initialize()
{
	Camera* cam;

	/* ============================================= Initial scene ======================================= */
	if (!SceneStorage::Get()->LoadScene("Scene1_10000_cubes"))
		return false;
	
	m_scene = SceneStorage::Get()->GetScene("Scene1_10000_cubes");
	m_scene->LoadIntoRenderManager();

	/* ======================================= Initial editor state ====================================== */
	m_editorMode = EditorModes::SELECTION;
	
	m_editorStateMachine = new StateMachine<EditorState>;
	m_editorStateMachine->Push<EditorSelectionState>(m_scene);
	if (!m_editorStateMachine->Peek()->Initialize())
		return false;

	/* ============================================= Cameras ============================================= */
	unsigned int nrOfObjects = m_scene->GetNrOfTexturedObjects();
	unsigned int nrOfObjectsPerSide = sqrt(nrOfObjects);

	Vector3f startPos = Vector3f((nrOfObjectsPerSide - 1) * -0.5f, 1.0f, (nrOfObjectsPerSide - 1) * -0.5f) * 4;

	cam = PlayerCameraManager::Get()->CreateCamera();
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

	// Look at origin
	if (input->IsKeyDown('R'))
	{
		manager->GetCurrentCamera()->SetTarget(0.0f, 0.0f, 0.0f);
		player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
	}

	// Switch, save, load scenes
	if (input->IsKeyDown(VK_CONTROL))
	{
		// Change scenes
		if (input->IsKeyPressed(48 + 1))
		{
			if (m_scene->GetName() != "Scene1_10000_cubes")
			{
				if (!SceneStorage::Get()->LoadScene("Scene1_10000_cubes"))
				{
					EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
					return;
					// Send event to leave editor
				}

				m_scene = SceneStorage::Get()->GetScene("Scene1_10000_cubes");
				m_scene->LoadIntoRenderManager();
			}
		}
		if (input->IsKeyPressed(48 + 2))
		{
			if (m_scene->GetName() != "Scene2_10000_turrets")
			{
				if (!SceneStorage::Get()->LoadScene("Scene2_10000_turrets"))
				{
					EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
					return;
					// Send event to leave editor
				}

				m_scene = SceneStorage::Get()->GetScene("Scene2_10000_turrets");
				m_scene->LoadIntoRenderManager();
			}
		}
		if (input->IsKeyPressed(48 + 3))
		{
			if (m_scene->GetName() != "Scene3_1_turret")
			{
				if (!SceneStorage::Get()->LoadScene("Scene3_1_turret"))
				{
					EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
					return;
					// Send event to leave editor
				}

				m_scene = SceneStorage::Get()->GetScene("Scene3_1_turret");
				m_scene->LoadIntoRenderManager();
			}
		}
		if (input->IsKeyPressed(48 + 4))
		{
			if (m_scene->GetName() != "Scene4_100_cubes")
			{
				if (!SceneStorage::Get()->LoadScene("Scene4_100_cubes"))
				{
					EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
					return;
					// Send event to leave editor
				}

				m_scene = SceneStorage::Get()->GetScene("Scene4_100_cubes");
				m_scene->LoadIntoRenderManager();
			}
		}

		// Load or save current scene
		if (input->IsKeyPressed('O'))
		{
			m_scene->SaveToFile(m_scene->GetName());
		}
		else if (input->IsKeyPressed('I'))
		{
			m_scene->LoadFromFile(m_scene->GetName());
		}
	}
	else
	{
		if (input->IsKeyPressed(48 + 1) && m_editorMode != EditorModes::SELECTION)
		{
			m_editorMode = EditorModes::SELECTION;

			m_editorStateMachine->Replace<EditorSelectionState>(m_scene);
			m_editorStateMachine->Peek()->Initialize();
		}
		if (input->IsKeyPressed(48 + 2) && m_editorMode != EditorModes::MOVE)
		{
			m_editorMode = EditorModes::MOVE;

			TextureObject* selectedObject = m_editorStateMachine->Peek()->GetSelectedObject();
			m_editorStateMachine->Replace<EditorMoveState>();
			
			m_editorStateMachine->Peek()->Initialize();
			m_editorStateMachine->Peek()->SetSelectedObject(selectedObject);
		}
		if (input->IsKeyPressed(48 + 3) && m_editorMode != EditorModes::PLACEMENT)
		{
			m_editorMode = EditorModes::PLACEMENT;

			m_editorStateMachine->Replace<EditorPlacementState>(m_scene);
			m_editorStateMachine->Peek()->Initialize();
		}
	}

	// Rotate camera
	
	if (!input->IsKeyDown(VK_LBUTTON))
	{
		if (mouseMovement != Vector2f(0, 0))
		{
			mouseMovement *= 0.015f;

			manager->GetCurrentCamera()->RotateUp(mouseMovement.y);
			manager->GetCurrentCamera()->RotateRight(mouseMovement.x);
			player->SetLookDirection(manager->GetCurrentCamera()->GetTargetDirection());
		}
	}

	// Process input of current editor state
	m_editorStateMachine->Peek()->ProcessInput();
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

	m_scene->Update(dt);

	m_editorStateMachine->Peek()->Update(dt);

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

	RenderHUDText();

	d3d->Present();
}

void SceneEditorState::RenderNormal()
{
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	Camera* cam0 = PlayerCameraManager::Get()->GetCamera(0);
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	bufferStorage->SetVSPointLight(deviceContext, cam0->GetPosition(), 1.0f);
	RenderManager::Get()->RenderTexturedObjects();

	bufferStorage->SetPSPointLight(deviceContext, cam0->GetPosition(), 1.0f);
	RenderManager::Get()->RenderSingleColoredObjects();
}

void SceneEditorState::RenderHUDText()
{
	Direct3D* d3d = Direct3D::Get();

	d3d->DisableZBuffer();

	m_fpsCounter.get()->Render();

	d3d->EnableZBuffer();
	d3d->SetDefaultBlendState();
}