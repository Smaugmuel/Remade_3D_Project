#ifndef ENGINE_HPP
#define ENGINE_HPP
//#include "../Misc/Singleton.hpp"
#include "../Math/Vector2.hpp"
#include <chrono>

#include "../FrameWork/FrameWorkManager.hpp"
#include "MaterialManager.hpp"
#include "ModelManager.hpp"
#include "SceneManagerV3.hpp"
#include "CameraManagerV2.hpp"
#include "../GUI/FPS_CounterV2.hpp"

class Engine final //: public Singleton<Engine>
{
public:
	Engine();
	~Engine();

	bool Initialize(Vector2i windowSize = Vector2i(800, 600));

	bool Update();

	void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void Render();
	void Present();

	void EnableFirstPersonControls();
	void DisableFirstPersonControls();

	void ShowFPSCounter();
	void HideFPSCounter();

	/*void EnableDepthTests();
	void DisableDepthTests();*/

	GUIManager* GetGUIManager();
	SceneManagerV3* GetSceneManager();
	CameraManagerV2* GetCameraManager();
	ModelManager* GetModelManager();
	MaterialManager* GetMaterialManager();
	Input* GetInput();

	Vector2i GetWindowSize() const;

private:
	//friend class Singleton<Engine>;

	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::time_point<std::chrono::steady_clock> Time;

	void ControlCamera(float dt);

	bool m_isRunning;

	Vector2i m_windowSize;

	int m_renderCameraIndex;
	bool m_hasFirstPersonControls;

	/*
	NOTE:
	The FPS counter has a pointer to the gui manager
	Upon FPS counter destruction, it calls one of gui manager's functions
	Because of this, AND because the gui manager is destroyed when the framework manager is destroyed,
	AND because elements are created in the order up->down and destroyed in the order down->up,
	the FPS counter must be created AFTER (not necessarily directly) the framework manager
	*/
	FrameWorkManager m_frameWorkManager;
	ModelManager m_modelManager;
	MaterialManager m_materialManager;
	SceneManagerV3 m_sceneManager;
	CameraManagerV2 m_cameraManager;

	FPSCounterV2 m_fps;
	Time m_t1;
	Time m_t2;
};

#endif