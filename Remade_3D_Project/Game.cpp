#include "Game.hpp"

//#include "../Engine/Lights/PointLightManager.hpp"
#include "../Engine/Math/Collision.hpp"
#include "../Engine/Events/EventDispatcher.hpp"

//#include "PlayState.hpp"
//#include "MainMenuState.hpp"
//#include "SceneStorage.hpp"
//#include "SceneEditorState.hpp"


Game::Game() : m_cameraIndex(-1), m_GUIhelloWorldIndex(-1)
{
	EventDispatcher::Get()->Subscribe(EventType::POP_GAMESTATE, this);
	EventDispatcher::Get()->Subscribe(EventType::FAILED_TO_INITIALIZE, this);
}

Game::~Game()
{
	EventDispatcher::Get()->Unsubscribe(EventType::POP_GAMESTATE, this);
	EventDispatcher::Get()->Unsubscribe(EventType::FAILED_TO_INITIALIZE, this);

	/*SceneStorage::Delete();*/

	EventDispatcher::Delete();

	//PointLightManager::Delete();
	Collision::Delete();
}

bool Game::Initialize()
{
	if (!m_engine.Initialize(Vector2i(1500, 800)))
		return false;
	
	/*if (!SceneStorage::Get()->LoadScene("Scene1"))
		return false;
	if (!SceneStorage::Get()->LoadScene("Scene3"))
		return false;
	if (!SceneStorage::Get()->LoadScene("Scene4"))
		return false;*/

#ifdef OLD_MODE
	//m_gameStateMachine.Push<SceneEditorState>();
	//m_gameStateMachine.Push<MainMenuState>();

	m_gameStateMachine.Push<PlayState>();
	if (!m_gameStateMachine.Peek()->Initialize())
		return false;

	m_pop_game_state_flag = false;
	m_shutdown_flag = false;
#endif

	m_cameraIndex = m_engine.GetCameraManager()->CreateCamera();
	m_engine.EnableFirstPersonControls();
	CameraV3* cam = m_engine.GetCameraManager()->GetCamera(m_cameraIndex);
	
	cam->position = Vector3f(0, 20, -128);
	cam->target = Vector3f(0, 0, 0);
	cam->up = Vector3f(0, 1, 0);
	cam->UpdateViewMatrix();
	cam->fov = 3.14159265f / 2.0f;
	cam->aspectRatio = m_engine.GetWindowSize().x / static_cast<float>(m_engine.GetWindowSize().y);
	cam->nearPlane = 0.01f;
	cam->farPlane = 1000.0f;
	cam->UpdateProjectionMatrix();
	m_engine.GetSceneManager()->SetViewAndProjectionMatrices(cam->viewMatrix, cam->projectionMatrix);

	const unsigned int nObjX = 100;
	const unsigned int nObjZ = 100;
	Vector3f startPos = Vector3f(static_cast<float>(nObjX), 0, static_cast<float>(nObjZ)) * -1.0f;
	Vector3f offset(4, 0, 4);

	for (int i = 0; i < nObjX; i++)
	{
		for (int j = 0; j < nObjZ; j++)
		{
			int index = m_engine.GetSceneManager()->CreateObject();
			if (index == -1)
				return false;

			ObjectV3* obj = m_engine.GetSceneManager()->GetObjectV3(index);
			obj->modelIndex = m_engine.GetModelManager()->LoadModel("SimpleCharacter.obj");
			if (obj->modelIndex == -1)
				return false;

			obj->position = startPos + Vector3f(offset.x * i, offset.y * 0, offset.z * j);
			obj->worldMatrix.SetTranslation(obj->position);
		}
	}

	m_GUIhelloWorldIndex = m_engine.GetGUIManager()->CreateText("hello world", Vector2i(0, 100), Fonts::COMIC_SANS_MS_16);
	if (m_GUIhelloWorldIndex == -1)
		return false;

	//SceneManager::Get()->SetRenderMode(RenderModes::NORMAL);

	m_engine.ShowFPSCounter();

	return true;
}

void Game::Run()
{
	SceneManagerV3* sceneManager = m_engine.GetSceneManager();
	//SceneManager* sceneManager = SceneManager::Get();

	Matrix rotMatrix = Matrix::Rotation(Vector3f(0, 1, 0), 0.01f);


	while (true)
	{
		/*if (m_pop_game_state_flag)
		{
			m_gameStateMachine.Pop();
			m_pop_game_state_flag = false;

			// Process initialization failures
			// (Could replace the Initialize() in most classes, since they won't need to return false)
			if (m_shutdown_flag)
			{
				while (!m_gameStateMachine.IsEmpty())
				{
					m_gameStateMachine.Pop();
					m_shutdown_flag = false;
				}
			}
		}*/

		/*if (Input::Get()->IsKeyPressed('T'))
		{
			sceneManager->SetObjectModel(5000, "generator.obj");
		}*/

		unsigned int n = sceneManager->GetNrOfObjects();
		for (unsigned int i = 0; i < n; i++)
		{
			ObjectV3* obj = sceneManager->GetObjectV3(i);
			obj->worldMatrix = rotMatrix * obj->worldMatrix;
		}

		if (!m_engine.Update())
			break;

		m_engine.Clear(0, 0, 0, 1);
		m_engine.Render();
		m_engine.Present();
	}
}

void Game::ReceiveEvent(const Event & e)
{
	/*switch (e.type)
	{
	case EventType::FAILED_TO_INITIALIZE:
		m_shutdown_flag = true;
	case EventType::POP_GAMESTATE:
		m_pop_game_state_flag = true;
		break;
	default:
		break;
	}*/
}