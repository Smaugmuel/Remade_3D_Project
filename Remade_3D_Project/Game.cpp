#include "Game.hpp"

//#include "../Engine/Math/Collision.hpp"
#include "../Engine/Events/EventDispatcher.hpp"
#include "../Engine/Math/Math.hpp"

#include <algorithm>

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
	
	cam->position = Vector3f(0, 20, -32);
	cam->target = Vector3f(0, 0, 0);
	cam->up = Vector3f(0, 1, 0);
	cam->UpdateViewMatrix();
	cam->fov = 3.14159265f / 2.0f;
	cam->aspectRatio = m_engine.GetWindowSize().x / static_cast<float>(m_engine.GetWindowSize().y);
	cam->nearPlane = 0.01f;
	cam->farPlane = 1000.0f;
	cam->UpdateProjectionMatrix();
	m_engine.GetSceneManager()->SetViewAndProjectionMatrices(cam->viewMatrix, cam->projectionMatrix);

	SceneManagerV3* sceneManager = m_engine.GetSceneManager();
	MaterialManager* materialManager = m_engine.GetMaterialManager();
	ModelManager* modelManager = m_engine.GetModelManager();

	int turretIndex = modelManager->LoadModel("turret.obj");
	int characterIndex = modelManager->LoadModel("SimpleCharacter.obj");
	int cubeIndex = modelManager->LoadModel("cube_uv.obj");
	int sphereIndex = modelManager->LoadModel("Sphere.obj");
	int sphereLPIndex = modelManager->LoadModel("Sphere_LowPoly_AvgNormals.obj");
	int cubeRedIndex = modelManager->LoadModel("cube_uv_red.obj");

	int turretTextureIndex = materialManager->GetMaterial(modelManager->GetModel(turretIndex)->materialIndex)->textureIndex;
	int characterTextureIndex = materialManager->GetMaterial(modelManager->GetModel(characterIndex)->materialIndex)->textureIndex;
	int cubeTextureIndex = materialManager->GetMaterial(modelManager->GetModel(cubeIndex)->materialIndex)->textureIndex;
	int sphereTextureIndex = materialManager->GetMaterial(modelManager->GetModel(sphereIndex)->materialIndex)->textureIndex;
	int sphereLPTextureIndex = materialManager->GetMaterial(modelManager->GetModel(sphereLPIndex)->materialIndex)->textureIndex;
	int cubeRedTextureIndex = materialManager->GetMaterial(modelManager->GetModel(cubeRedIndex)->materialIndex)->textureIndex;


	ObjectData* objectData = sceneManager->GetData()->GetObjectData();
	Vector3f* positions = objectData->positions;
	Vector3f* scales = objectData->scales;
	Math::Matrix* rotationMatrices = objectData->rotationMatrices;
	Math::Matrix* worldMatrices = objectData->worldMatrices;
	int* modelIndices = objectData->modelIndices;
	int* textureIndices = objectData->textureIndices;


	const unsigned int nObjX = 64;
	const unsigned int nObjZ = 32;
	Vector3f offset(4, 0, 4);
	Vector3f startPos = Vector3f(static_cast<float>(nObjX) * offset.x, 0.0f, static_cast<float>(nObjZ) * offset.z) * -0.5f;

	for (int i = 0; i < nObjX; i++)
	{
		for (int j = 0; j < nObjZ; j++)
		{
			int modelIndex = 0 % 2 == 0 ? turretIndex : characterIndex;
			int textureIndex = 0 % 2 == 0 ? turretTextureIndex : characterTextureIndex;
			/*int modelIndex = sphereIndex;
			int textureIndex = sphereTextureIndex;*/
			/*int modelIndex = cubeTextureIndex;
			int textureIndex = cubeTextureIndex;*/

			ObjectV4* object = sceneManager->CreateObject();
			if (!object)
				return false;
			m_objects.push_back(object);
			
			*object->positionPtr = startPos + Vector3f(offset.x * i, offset.y * 0, offset.z * j);
			*object->scalePtr = Vector3f(1, 1, 1);
			*object->worldMatrixPtr = Math::Matrix::World(*object->positionPtr, *object->rotationMatrixPtr, *object->scalePtr);
			*object->modelIndexPtr = modelIndex;
			*object->textureIndexPtr = textureIndex;
			*object->movementPtr = Vector3f(0, 0, 0);
			*object->rotationAxisPtr = Vector3f(1, 0, 0);
			*object->rotationSpeedPtr = 1.0f;
		}
	}

	ObjectV4* object = sceneManager->CreateObject();
	if (!object)
		return false;
	m_objects.push_back(object);

	*object->positionPtr = Vector3f(0, -15, 0);
	*object->scalePtr = Vector3f(nObjX * 2, 1, nObjZ * 2);
	*object->worldMatrixPtr = Math::Matrix::World(*object->positionPtr, *object->rotationMatrixPtr, *object->scalePtr);
	*object->modelIndexPtr = cubeRedIndex;
	*object->textureIndexPtr = cubeRedTextureIndex;
	*object->movementPtr = Vector3f(0, 0, 0);
	*object->rotationAxisPtr = Vector3f(1, 0, 0);
	*object->rotationSpeedPtr = 0.0f;






	/*
	Create lights
	*/
	LightData* lightData = sceneManager->GetData()->GetLightData();
	//lightData->ambientColor = Vector3f(0.05f, 0.05f, 0.05f);
	lightData->ambientColor = Vector3f(1.0f, 1.0f, 1.0f);

	for (unsigned int i = 0; i < 3U; i++)
	{
		Light* light = sceneManager->GetData()->CreateLight();
		if (!light)
			return false;
		*light->diffuseColorPtr = Vector3f(1, 1, 1);
		*light->dropoffPtr = -0.01f;
	}
	lightData->positions[0] = Vector3f(-100, 0, 0);
	lightData->positions[1] = Vector3f(0, 0, 0);
	lightData->positions[2] = Vector3f(100, 0, 0);

	m_GUIhelloWorldIndex = m_engine.GetGUIManager()->CreateText("hello world", Vector2i(0, 100), Fonts::COMIC_SANS_MS_16);
	if (m_GUIhelloWorldIndex == -1)
		return false;

	m_engine.ShowFPSCounter();

	return true;
}

void Game::Run()
{
	SceneManagerV3* sceneManager = m_engine.GetSceneManager();
	DataStorage* dataStorage = sceneManager->GetData();
	ObjectData* objectData = dataStorage->GetObjectData();

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


		if (!m_engine.Update())
			break;
		
		if (m_engine.GetInput()->IsKeyDown('E'))
		{
			CameraV3* camera = m_engine.GetCameraManager()->GetCamera(m_cameraIndex);
			Math::Ray ray = Math::CalculatePickingRay(camera->viewMatrix, camera->projectionMatrix, m_engine.GetInput()->MousePosition(), m_engine.GetWindowSize());
	
			objectData->positions[0] = ray.origin + ray.direction * 10;

			Math::Sphere sphere;
			Math::OBB obb;
			sphere.position = objectData->positions[0];
			obb.center = objectData->positions[m_objects.size() - 1];
			obb.halfSides[0] = objectData->scales[m_objects.size() - 1].x;
			obb.halfSides[1] = objectData->scales[m_objects.size() - 1].y;
			obb.halfSides[2] = objectData->scales[m_objects.size() - 1].z;
			if (Math::Collision::SphereVsOBB(sphere, obb))
			{
				objectData->textureIndices[0] = 0;
			}
			else
			{
				objectData->textureIndices[0] = 1;
			}
		}


		if (m_engine.GetInput()->IsKeyPressed('G'))
		{
			for (unsigned int i = 0; i < 3; i++)
			{
				dataStorage->GetLightData()->positions[i] = Vector3f(0, 10, 0);
			}
			dataStorage->GetLightData()->diffuseColors[0] = Vector3f(1, 0, 0);
			dataStorage->GetLightData()->diffuseColors[1] = Vector3f(0, 1, 0);
			dataStorage->GetLightData()->diffuseColors[2] = Vector3f(0, 0, 1);
		}

		if (m_engine.GetInput()->IsKeyPressed('R'))
		{
			Vector3f* positions = objectData->positions;
			Vector3f* movements = objectData->movements;
			unsigned int nrOfObjects = dataStorage->GetNrOfObjects();
			for (unsigned int i = 0; i < nrOfObjects; i++)
			{
				positions[i] = Vector3f(0, 0, 0);
				movements[i] = Vector3f(0, 0, 0);
			}
		}
		if (m_engine.GetInput()->IsKeyPressed('T'))
		{
			Vector3f* movements = objectData->movements;
			Vector3f* rotationAxises = objectData->rotationAxises;

			unsigned int nrOfObjects = dataStorage->GetNrOfObjects();
			for (unsigned int i = 0; i < nrOfObjects; i++)
			{
				float randX = (static_cast<float>(rand() % 1000) * 0.002f) - 1.0f;
				float randY = (static_cast<float>(rand() % 1000) * 0.002f) - 1.0f;
				float randZ = (static_cast<float>(rand() % 1000) * 0.002f) - 1.0f;

				movements[i] = Vector3f(randX, randY, randZ).normalized() * 10.0f;
				rotationAxises[i] = movements[i].crossLH(Vector3f(0, 1, 0));
			}
		}

		if (m_engine.GetInput()->IsKeyPressed(VK_LBUTTON))
		{
			unsigned int nrOfObjects = dataStorage->GetNrOfObjects();
			if (nrOfObjects > 0)
			{
				CameraV3* camera = m_engine.GetCameraManager()->GetCamera(m_cameraIndex);
				Math::Ray ray = Math::CalculatePickingRay(camera->viewMatrix, camera->projectionMatrix, m_engine.GetInput()->MousePosition(), m_engine.GetWindowSize());

				Vector3f* positions = objectData->positions;
				Vector3f* movements = objectData->movements;
				Vector3f* rotationAxises = objectData->rotationAxises;
				int* textures = objectData->textureIndices;
				Math::Sphere sphere;

				float minDistance = INFINITY;
				int minDistanceIndex = -1;

				// Find the closest intersection
				for (unsigned int i = 0; i < nrOfObjects; i++)
				{
					sphere.position = positions[i];

					Math::Collision::RayIntersection intersection = Math::Collision::RayVsSphere(ray, sphere);
					if (intersection.collisionOccured)
					{
						if (intersection.distance < minDistance)
						{
							minDistance = intersection.distance;
							minDistanceIndex = i;
						}
					}
				}

				// Change the movement of the closest intersection, if any
				if (minDistanceIndex != -1)
				{
					float randX = (static_cast<float>(rand() % 1000) * 0.002f) - 1.0f;
					float randY = (static_cast<float>(rand() % 1000) * 0.002f) - 1.0f;
					float randZ = (static_cast<float>(rand() % 1000) * 0.002f) - 1.0f;
					movements[minDistanceIndex] = Vector3f(randX, randY, randZ).normalized() * 10.0f;
					textures[minDistanceIndex] = 0;
				}
			}
		}

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