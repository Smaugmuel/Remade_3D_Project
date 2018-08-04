#include "FPS_CounterV2.hpp"
#include "../GUI/GUIManager.hpp"
#include <string>

FPSCounterV2::FPSCounterV2() : m_id(-1), m_timeElapsed(0.0f), m_nrOfUpdates(0)
{
}

FPSCounterV2::~FPSCounterV2()
{
}

int FPSCounterV2::CreateGUIText()
{
	m_id = GUIManager::Get()->CreateText("FPS: 0", Vector2i(4, 4));
	return m_id;
}

void FPSCounterV2::RemoveGUIText()
{
	GUIManager::Get()->RemoveText(m_id);
}

void FPSCounterV2::Show()
{
	GUIManager::Get()->SetTextFlag(m_id, GUI_Flags::IS_RENDERED, true);
}

void FPSCounterV2::Hide()
{
	GUIManager::Get()->SetTextFlag(m_id, GUI_Flags::IS_RENDERED, false);
}

void FPSCounterV2::Update(float dt)
{
	m_timeElapsed += dt;
	m_nrOfUpdates++;

	if (m_timeElapsed >= 1.0f)
	{
		GUIManager::Get()->SetText(m_id, "FPS: " + std::to_string(m_nrOfUpdates));

		m_timeElapsed = 0.0f;
		m_nrOfUpdates = 0;
	}
}

int FPSCounterV2::GetGUIID() const
{
	return m_id;
}