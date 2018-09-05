#include "FPS_CounterV2.hpp"
#include "../FrameWork/GUIManager.hpp"
#include <string>

FPSCounterV2::FPSCounterV2() : m_id(-1), m_timeElapsed(0.0f), m_nrOfUpdates(0), m_guiManager(nullptr)
{
}

FPSCounterV2::~FPSCounterV2()
{
	if (m_guiManager && m_id != -1)
	{
		m_guiManager->RemoveText(m_id);
	}
}

bool FPSCounterV2::Initialize(GUIManager * guiManager)
{
	m_guiManager = guiManager;
	m_id = m_guiManager->CreateText("FPS: 0", Vector2i(4, 4));
	if (m_id == -1)
		return false;

	return true;
}

void FPSCounterV2::Show()
{
	m_guiManager->SetTextFlag(m_id, GUI_Flags::IS_RENDERED, true);
}

void FPSCounterV2::Hide()
{
	m_guiManager->SetTextFlag(m_id, GUI_Flags::IS_RENDERED, false);
}

void FPSCounterV2::Update(float dt)
{
	m_timeElapsed += dt;
	m_nrOfUpdates++;

	if (m_timeElapsed >= 1.0f)
	{
		m_guiManager->SetText(m_id, "FPS: " + std::to_string(m_nrOfUpdates));

		m_timeElapsed = 0.0f;
		m_nrOfUpdates = 0;
	}
}

int FPSCounterV2::GetGUIID() const
{
	return m_id;
}