#include "FPS_Counter.hpp"
#include <string>

FPS_Counter::FPS_Counter() : HUDText()
{
	m_timeElapsed = 0.0;
	m_nrOfUpdates = 0;
}

FPS_Counter::~FPS_Counter()
{
}

void FPS_Counter::Update(double dt)
{
	m_timeElapsed += dt;
	m_nrOfUpdates++;

	if (m_timeElapsed > 1.0)
	{
		HUDText::SetText(std::to_string(m_nrOfUpdates));

		m_nrOfUpdates = 0;
		m_timeElapsed = 0.0;
	}
}