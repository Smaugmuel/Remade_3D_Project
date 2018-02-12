#include "FPS_Counter.hpp"
#include "HUDText.hpp"
#include <string>

FPSCounter::FPSCounter()
{
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_text = std::make_unique<HUDText>();
	m_text->LoadFont(device, "../Fonts/courier32.spritefont");
	m_text->CreateSpriteBatch(deviceContext);
	m_text->SetText("FPS: ");
	m_text->SetPosition(Vector2f(4, 4));

	m_timeElapsed = 0.0f;
	m_nrOfUpdates = 0;
}

void FPSCounter::Update(float dt)
{
	m_timeElapsed += dt;
	m_nrOfUpdates++;

	if (m_timeElapsed >= 1.0f)
	{
		std::string text("FPS: " + std::to_string(m_nrOfUpdates));
		m_text->SetText(text.c_str());

		m_timeElapsed = 0.0f;
		m_nrOfUpdates = 0;
	}
}

void FPSCounter::Render()
{
	m_text->Render();
}