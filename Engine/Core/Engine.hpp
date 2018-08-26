#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "../Misc/Singleton.hpp"
#include "../Math/Vector2.hpp"
#include <chrono>

#include "../FrameWork/FrameWorkManager.hpp"
#include "MaterialManager.hpp"
#include "ModelManager.hpp"
#include "SceneManagerV3.hpp"
#include "CameraManagerV2.hpp"
#include "../GUI/FPS_CounterV2.hpp"
//#include "../Camera/CameraV2.hpp"

class Engine final : public Singleton<Engine>
{
public:
	bool Initialize(Vector2i windowSize = Vector2i(800, 600));

	bool Update();

	void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void Render();
	void Present();

	void ShowFPSCounter();
	void HideFPSCounter();

	/*void EnableDepthTests();
	void DisableDepthTests();*/

	GUIManager* GetGUIManager();
	SceneManagerV3* GetSceneManager();
	CameraManagerV2* GetCameraManager();
	ModelManager* GetModelManager();
	MaterialManager* GetMaterialManager();

	Vector2i GetWindowSize() const;

private:
	friend class Singleton<Engine>;

	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::time_point<std::chrono::steady_clock> Time;

	Engine();
	~Engine();

	bool m_isRunning;

	Vector2i m_windowSize;

	FPSCounterV2 m_fps;

	Time m_t1;
	Time m_t2;

	FrameWorkManager m_frameWorkManager;
	ModelManager m_modelManager;
	MaterialManager m_materialManager;
	SceneManagerV3 m_sceneManager;
	CameraManagerV2 m_cameraManager;
	//ReusableStorage<CameraV2> m_cameras;
};

#endif