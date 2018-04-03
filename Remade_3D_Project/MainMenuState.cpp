#include "MainMenuState.hpp"

#include "StateMachine.hpp"
#include "PlayState.hpp"
#include "SceneEditorState.hpp"

#include "EventDispatcher.hpp"

#include "Input.hpp"
#include <Windows.h>

// For moving the buttons
#include "WindowSettings.hpp"

// For rendering the text of the buttons
#include "Menu.hpp"
#include "Direct3D.hpp"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "SimpleMath.h"
//#include <locale>
//#include <codecvt>

MainMenuState::MainMenuState(StateMachine<GameState>* stateMachine) : GameState::GameState(stateMachine)
{
}
MainMenuState::~MainMenuState()
{
	if (m_menu)
	{
		delete m_menu;
		m_menu = nullptr;
	}
	if (m_spriteFont)
	{
		delete m_spriteFont;
		m_spriteFont = nullptr;
	}
	if (m_spriteBatch)
	{
		delete m_spriteBatch;
		m_spriteBatch = nullptr;
	}
}

bool MainMenuState::Initialize()
{
	DirectX::SimpleMath::Vector2 halfDimensions;

	// Create the menu
	m_menu = new Menu<MainMenuState>;

	// Create the resources needed for rendering the text of the buttons
	m_spriteFont = new DirectX::SpriteFont(Direct3D::Get()->GetDevice(), L"../Fonts/courier32.spritefont");
	m_spriteBatch = new DirectX::SpriteBatch(Direct3D::Get()->GetDeviceContext());


	// Calculate the size of the PLAY button, then create it
	halfDimensions = DirectX::SimpleMath::Vector2(m_spriteFont->MeasureString(L"PLAY")) / 2.0f;
	m_menu->CreateButton(this, &MainMenuState::PressedPlay, L"PLAY", Vector2i(400, 100), Vector2i(halfDimensions.x, halfDimensions.y));

	// Calculate the size of the EDITOR button, then create it
	halfDimensions = DirectX::SimpleMath::Vector2(m_spriteFont->MeasureString(L"EDITOR")) / 2.0f;
	m_menu->CreateButton(this, &MainMenuState::PressedEdit, L"EDITOR", Vector2i(400, 300), Vector2i(halfDimensions.x, halfDimensions.y));

	// Calculate the size of the EXIT button, then create it
	halfDimensions = DirectX::SimpleMath::Vector2(m_spriteFont->MeasureString(L"EXIT")) / 2.0f;
	m_menu->CreateButton(this, &MainMenuState::PressedExit, L"EXIT", Vector2i(400, 500), Vector2i(halfDimensions.x, halfDimensions.y));

	return true;
}

void MainMenuState::ProcessInput()
{
	Input* input = Input::Get();

	if (input->IsKeyPressed(VK_ESCAPE))
	{
		EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
	}

	if (input->IsKeyPressed(VK_LBUTTON))
	{
		m_menu->ProcessMouseClick(input->MousePosition());
	}
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

	std::vector<MenuButton<MainMenuState>>& buttons = m_menu->RetrieveButtons();
	unsigned int n = buttons.size();

	if (n > 0)
	{
		float inverseN = 1.0f / n;
		for (unsigned int i = 0; i < n; i++)
		{
			float _angle = angle + 3.1415927f * 2.0f * inverseN * i;

			buttons[i].SetPosition(
				Vector2i(
					(WNDW >> 1) + std::cosf(_angle) * (WNDW >> 2),
					(WNDH >> 1) + std::sinf(_angle) * (WNDH >> 2)
				)
			);
		}
	}
}

void MainMenuState::Render()
{
	const std::vector<MenuButton<MainMenuState>>& buttons = m_menu->RetrieveButtons();
	Direct3D* d3d = Direct3D::Get();

	// Prepare for rendering
	d3d->ClearDefaultTarget();
	d3d->DisableZBuffer();
	m_spriteBatch->Begin();

	// Render text of buttons
	unsigned int n = buttons.size();
	for (unsigned int i = 0; i < n; i++)
	{
		// Convert position and size to SimpleMath vector
		const MenuButton<MainMenuState>* button = &buttons[i];
		const AABA& aaba = button->GetAABA();
		DirectX::SimpleMath::Vector2 position(aaba.center.x, aaba.center.y);
		DirectX::SimpleMath::Vector2 origin(aaba.halfSides.x, aaba.halfSides.y);

		m_spriteFont->DrawString(m_spriteBatch, button->GetText().c_str(), position, DirectX::Colors::White, 0.0f, origin);
	}

	// Reset states and present scene
	m_spriteBatch->End();
	d3d->EnableZBuffer();
	d3d->SetDefaultBlendState();
	d3d->Present();
}

void MainMenuState::PressedPlay()
{
	m_stateMachine->Push<PlayState>();
	if (!m_stateMachine->Peek()->Initialize())
	{
		EventDispatcher::Get()->Emit(Event(EventType::FAILED_TO_INITIALIZE));
	}
}
void MainMenuState::PressedEdit()
{
	m_stateMachine->Push<SceneEditorState>();
	if (!m_stateMachine->Peek()->Initialize())
	{
		EventDispatcher::Get()->Emit(Event(EventType::FAILED_TO_INITIALIZE));
	}
}
void MainMenuState::PressedExit()
{
	EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
}