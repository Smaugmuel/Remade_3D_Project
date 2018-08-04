#include "FPS_Counter.hpp"
#include "../GUI/HUDText.hpp"
#include <string>
#include "../Render/Direct3D.hpp"

FPSCounter::FPSCounter()
{
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::Initialize()
{

	m_text = std::make_unique<HUDText>();
	
	// These rows are needed, just commented for now
	//m_text->LoadFont(Direct3D::Get()->GetDevice(), "../Fonts/courier32.spritefont");
	//m_text->CreateSpriteBatch(Direct3D::Get()->GetDeviceContext());
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