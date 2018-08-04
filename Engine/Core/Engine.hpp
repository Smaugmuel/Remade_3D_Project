#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "../Misc/Singleton.hpp"
#include "../Math/Vector2.hpp"
#include <memory>
#include <chrono>

#include "../FrameWork/FrameWork.hpp"

class FPSCounterV2;

class Engine final : public Singleton<Engine>
{
public:
	bool Initialize(int windowSizeX = 800, int windowSizeY = 600);
	bool Initialize(Vector2i windowSize = Vector2i(800, 600));

	bool Update();

	void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void Present();

	void ShowFPSCounter();
	void HideFPSCounter();

	void EnableDepthTests();
	void DisableDepthTests();

	Vector2i GetWindowSize() const;

private:
	friend class Singleton<Engine>;

	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::time_point<std::chrono::steady_clock> Time;

	Engine();
	~Engine();

	bool m_isRunning;

	Vector2i m_windowSize;

	std::unique_ptr<FPSCounterV2> m_fps;
	int m_fpsID;

	Time m_t1;
	Time m_t2;



	FrameWork m_frameWork;
};

#endif