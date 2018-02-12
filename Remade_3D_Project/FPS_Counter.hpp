#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;
class HUDText;

class FPSCounter
{
public:
	FPSCounter();
	~FPSCounter();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void Update(float dt);
	void Render();

private:
	std::unique_ptr<HUDText> m_text;

	float m_timeElapsed;
	unsigned int m_nrOfUpdates;
};

#endif