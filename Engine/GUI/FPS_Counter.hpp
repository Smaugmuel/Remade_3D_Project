#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP
#include <memory>

class HUDText;

class FPSCounter final
{
public:
	FPSCounter();
	~FPSCounter();

	void Initialize();

	void Update(float dt);
	void Render();

private:
	std::unique_ptr<HUDText> m_text;

	float m_timeElapsed;
	unsigned int m_nrOfUpdates;
};

#endif