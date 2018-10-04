#include "World.hpp"

#include "../Engine/FrameWork/Window.hpp"
#include "../Engine/FrameWork/Direct3D.hpp"
#include "../Engine/Buffers/ConstantBufferStorage.hpp"
#include "../Engine/Camera/Camera.hpp"
#include "../Engine/Camera/PlayerCameraManager.hpp"
#include "../Engine/Events/EventDispatcher.hpp"
//#include "../Engine/Misc/FPS_Counter.hpp"
#include "../Engine/Lights/PointLightManager.hpp"

#include "Scene.hpp"
#include "SceneStorage.hpp"
#include "Character.hpp"

World* Singleton<World>::s_instance = nullptr;

World::World()
{
	/* ============================================= Initial scene ======================================= */
	if (!SceneStorage::Get()->LoadScene("Scene4"))
	{
		EventDispatcher::Get()->Emit(Event(EventType::FAILED_TO_INITIALIZE));
	}

	m_scene = SceneStorage::Get()->GetScene("Scene4");
	m_scene->LoadIntoRenderManager();

	EventDispatcher::Get()->Emit(Event(EventType::SWITCHED_SCENE, static_cast<void*>(m_scene)));

	/* ============================================= Cameras ============================================= */
	unsigned int nrOfObjects = m_scene->GetNrOfTexturedObjects();
	unsigned int nrOfObjectsPerSide = static_cast<unsigned int>(sqrt(nrOfObjects));

	Vector3f startPos = Vector3f((nrOfObjectsPerSide - 1) * -0.5f, 1.0f, (nrOfObjectsPerSide - 1) * -0.5f) * 4;

	/*Camera* cam = PlayerCameraManager::Get()->CreateCamera();
	cam->SetDimensions(Window::Get()->GetDimensions());
	cam->SetPosition(startPos + Vector3f(0, 5, -5));
	cam->SetTarget(startPos);
	cam->Update();*/

	/* ============================================= Player character ============================================== */
	m_player = std::make_unique<Character>();
	m_player.get()->SetMovementSpeed(5.0f);
	/*m_player.get()->SetPosition(PlayerCameraManager::Get()->GetCurrentCamera()->GetPosition());
	m_player.get()->SetLookDirection(PlayerCameraManager::Get()->GetCurrentCamera()->GetTargetDirection());*/

	/* ============================================ FPS counter ==================================================== */
	//m_fpsCounter = std::make_unique<FPSCounter>();
}

World::~World()
{
}

void World::Update(float dt)
{
	static float angle = 0.0f;
	unsigned int n;

	PointLightManager* lightManager = PointLightManager::Get();
	/*PlayerCameraManager* cameraManager = PlayerCameraManager::Get();
	Camera* cam = cameraManager->GetCurrentCamera();*/

	angle += dt;

	m_player.get()->Update(dt);

	/*cam->SetPosition(m_player.get()->GetPosition());
	cam->SetTarget(m_player.get()->GetPosition() + m_player.get()->GetLookDirection());
	cameraManager->Update();*/

	n = lightManager->GetNrOfPointLights();
	for (unsigned int i = 0; i < n; i++)
	{
		lightManager->GetPointLight(i)->SetPosition(Vector3f(std::cosf(angle + i * 0.5f), 1.0f, std::sinf(angle + i * 0.5f)) * 100.0f);
		lightManager->GetPointLight(i)->SetTarget(Vector3f(0, 0, 0));
		lightManager->GetPointLight(i)->Update();
	}

	m_scene->Update(dt);

	//m_fpsCounter.get()->Update(dt);

	//ConstantBufferStorage::Get()->SetVSViewMatrix(Direct3D::Get()->GetDeviceContext(), cam->GetViewMatrix());
}

void World::SetScene(Scene * scene)
{
	m_scene = scene;
}

Scene * World::GetScene()
{
	return m_scene;
}
Character * World::GetPlayer()
{
	return m_player.get();
}
//FPSCounter * World::GetFPSCounter()
//{
//	return m_fpsCounter.get();
//}
