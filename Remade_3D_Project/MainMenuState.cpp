#include "MainMenuState.hpp"

#include "../Engine/FrameWork/WindowSettings.hpp"

#include "../Engine/Events/EventDispatcher.hpp"
//#include "../Engine/Input/Input.hpp"

//#include "../Engine/GUI/GUIManager.hpp"
#include "../Engine/Misc/StringConverter.hpp"

#include "../Engine/Core/Engine.hpp"

#include "PlayState.hpp"
#include "SceneEditorState.hpp"
#include "TextButtonManager.hpp"
#include <Windows.h>

MainMenuState::MainMenuState(StateMachineV2<GameState>* stateMachine) : GameState::GameState(stateMachine)
{
	// Create the menu
	m_buttonManager = new TextButtonManager<MainMenuState>;

	// Calculate the size of the PLAY button, then create it
	/*Vector2i halfDimensions = GUIManager::Get()->GetDimensionsOfText(L"PLAY") * 0.5;
	m_buttonManager->CreateButton(this, &MainMenuState::PressedPlay, L"PLAY", Vector2i(400, 100), halfDimensions);

	// Calculate the size of the EDITOR button, then create it
	halfDimensions = GUIManager::Get()->GetDimensionsOfText(L"EDITOR") * 0.5;
	m_buttonManager->CreateButton(this, &MainMenuState::PressedEdit, L"EDITOR", Vector2i(400, 300), halfDimensions);

	// Calculate the size of the EXIT button, then create it
	halfDimensions = GUIManager::Get()->GetDimensionsOfText(L"EXIT") * 0.5;
	m_buttonManager->CreateButton(this, &MainMenuState::PressedExit, L"EXIT", Vector2i(400, 500), halfDimensions);*/
}
MainMenuState::~MainMenuState()
{
	if (m_buttonManager)
	{
		delete m_buttonManager;
		m_buttonManager = nullptr;
	}
}

void MainMenuState::OnEntry()
{
	m_buttonManager->EnableRendering();
	//Engine::Get()->HideFPSCounter();
}

void MainMenuState::OnExit()
{
	m_buttonManager->DisableRendering();
	//Engine::Get()->ShowFPSCounter();
}

bool MainMenuState::Initialize()
{
	//// Create the menu
	//m_buttonManager = new TextButtonManager<MainMenuState>;

	//// Calculate the size of the PLAY button, then create it
	//Vector2i halfDimensions = GUIManager::Get()->GetDimensionsOfText(L"PLAY") * 0.5;
	//m_buttonManager->CreateButton(this, &MainMenuState::PressedPlay, L"PLAY", Vector2i(400, 100), halfDimensions);

	//// Calculate the size of the EDITOR button, then create it
	//halfDimensions = GUIManager::Get()->GetDimensionsOfText(L"EDITOR") * 0.5;
	//m_buttonManager->CreateButton(this, &MainMenuState::PressedEdit, L"EDITOR", Vector2i(400, 300), halfDimensions);

	//// Calculate the size of the EXIT button, then create it
	//halfDimensions = GUIManager::Get()->GetDimensionsOfText(L"EXIT") * 0.5;
	//m_buttonManager->CreateButton(this, &MainMenuState::PressedExit, L"EXIT", Vector2i(400, 500), halfDimensions);

	return true;
}

void MainMenuState::ProcessInput()
{
	/*Input* input = Input::Get();

	if (input->IsKeyPressed(VK_ESCAPE))
	{
		EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
	}

	if (input->IsKeyPressed(VK_LBUTTON))
	{
		m_buttonManager->ProcessMouseClick(input->MousePosition());
	}*/
}

void MainMenuState::Update(float dt)
{
	// Rotate the menu buttons around the center

	static float angle = 0.0f;
	static const float timeToCompleteOneLap = 10.0f;

	// Increase angle and wrap it to [0, 2PI]
	angle += 3.1415927f * 2.0f / timeToCompleteOneLap * dt;
	if (angle > 3.1415927f * 2.0f)
	{
		angle -= 3.1415927f * 2.0f;
	}

	std::vector<TextMenuButton<MainMenuState>>& buttons = m_buttonManager->RetrieveButtons();
	unsigned int n = buttons.size();

	if (n > 0)
	{
		float inverseN = 1.0f / n;
		for (unsigned int i = 0; i < n; i++)
		{
			float _angle = angle + 3.1415927f * 2.0f * inverseN * i;

			Vector2i pos(
				(WNDW >> 1) + static_cast<int>(std::cosf(_angle) * (WNDW >> 2)),
				(WNDH >> 1) + static_cast<int>(std::sinf(_angle) * (WNDH >> 2))
			);

			buttons[i].SetPosition(pos);
		}
	}
}

void MainMenuState::Render()
{
}

void MainMenuState::PressedPlay(const std::wstring& text)
{
	m_stateMachine->Push<PlayState>();
	if (!m_stateMachine->Peek()->Initialize())
	{
		EventDispatcher::Get()->Emit(Event(EventType::FAILED_TO_INITIALIZE));
	}
}
void MainMenuState::PressedEdit(const std::wstring& text)
{
	m_stateMachine->Push<SceneEditorState>();
	if (!m_stateMachine->Peek()->Initialize())
	{
		EventDispatcher::Get()->Emit(Event(EventType::FAILED_TO_INITIALIZE));
	}
}
void MainMenuState::PressedExit(const std::wstring& text)
{
	EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
}