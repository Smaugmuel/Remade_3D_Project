#include "EditorLoadState.hpp"

// For input
#include "Input.hpp"

// For loading files
#include "StringConverter.hpp"

// For the load icon
#include "HUDObject.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"

// For the scene menu
#include "Menu.hpp"
#include "Direct3D.hpp"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "SimpleMath.h"
#include "WindowSettings.hpp"

// For loading scenes
#include "SceneStorage.hpp"
#include "Scene.hpp"

// For error management
#include "EventDispatcher.hpp"

EditorLoadState::EditorLoadState() : EditorState::EditorState(), m_loadIcon(nullptr), m_sceneMenu(nullptr)
{
}

EditorLoadState::~EditorLoadState()
{
	if (m_loadIcon)
	{
		delete m_loadIcon;
		m_loadIcon = nullptr;
	}
	if (m_sceneMenu)
	{
		delete m_sceneMenu;
		m_sceneMenu = nullptr;
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

bool EditorLoadState::Initialize()
{
	/* ============================================= Create the load icon ========================================== */
	m_loadIcon = new HUDObject;
	if (!m_loadIcon->Initialize(Direct3D::Get()->GetDevice(), "Icons/LoadIcon.png", Vector2i(300, 300), Vector2i(32, 32)))
	{
		return false;
	}
	m_loadIcon->SetPosition(Vector2i(0, 64));
	m_loadIcon->SetDimensions(Vector2i(32, 32));

	// Create the resources needed for rendering the text of the buttons
	m_spriteFont = new DirectX::SpriteFont(Direct3D::Get()->GetDevice(), L"../Fonts/courier32.spritefont");
	m_spriteBatch = new DirectX::SpriteBatch(Direct3D::Get()->GetDeviceContext());

	// Create menu and its buttons from the scenes found in the directory
	m_sceneMenu = new Menu<EditorLoadState, const std::string&>;
	CreateButtonsFromScenesInFolder();

	return true;
}

void EditorLoadState::ProcessInput()
{
	Input* input = Input::Get();

	if (input->IsKeyPressed(VK_LBUTTON))
	{
		const std::vector<MenuButton<EditorLoadState, const std::string&>>& buttons = m_sceneMenu->RetrieveButtons();
		
		// Process buttons manually, since the names need to be known
		// Would it be fine to always send a button's name when it calls its function?
		// It would require each callback function to receive at least a string
		for (unsigned int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i].GetAABA().Contains(input->MousePosition()))
			{
				// Load the chosen scene
				LoadScene(ToString(buttons[i].GetText()));
				break;
			}
		}
	}

	if (input->IsKeyDown(VK_CONTROL))
	{
		if (input->IsKeyPressed('R'))
		{
			CreateButtonsFromScenesInFolder();
		}
	}
}

void EditorLoadState::Update(float dt)
{
}

void EditorLoadState::Render()
{
}

void EditorLoadState::RenderHUD()
{
	Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	const std::vector<MenuButton<EditorLoadState, const std::string&>>& buttons = m_sceneMenu->RetrieveButtons();

	/* ================================ Render load icon ================================= */
	ID3D11ShaderResourceView* iconTexture = m_loadIcon->GetShaderResourceView();
	deviceContext->PSSetShaderResources(0, 1, &iconTexture);

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::HUD);

	m_loadIcon->Render(deviceContext);

	
	/* ================================ Render scene buttons ================================= */
	d3d->DisableZBuffer();
	m_spriteBatch->Begin();

	// Render text of buttons
	unsigned int n = buttons.size();
	for (unsigned int i = 0; i < n; i++)
	{
		// Convert position and size to SimpleMath vector
		const MenuButton<EditorLoadState, const std::string&>* button = &buttons[i];
		const AABA& aaba = button->GetAABA();
		DirectX::SimpleMath::Vector2 position(aaba.center.x, aaba.center.y);
		DirectX::SimpleMath::Vector2 origin(aaba.halfSides.x, aaba.halfSides.y);

		m_spriteFont->DrawString(m_spriteBatch, button->GetText().c_str(), position, DirectX::Colors::White, 0.0f, origin);
	}

	// Reset states and present scene
	m_spriteBatch->End();
	d3d->EnableZBuffer();
	d3d->SetDefaultBlendState();
}

void EditorLoadState::CreateButtonsFromScenesInFolder()
{
	unsigned int n = 0;
	float inverseN = 0;
	Vector2i position;
	DirectX::SimpleMath::Vector2 halfDimensions;

	/* ============================ Retrieve the names of all scenes in the directory ============================= */
	SceneStorage::Get()->UpdateExistingSceneNamesFromFolder();
	const std::vector<std::wstring>& wSceneNames = SceneStorage::Get()->GetExistingSceneNames();

	// Do nothing if no scenes exist
	n = wSceneNames.size();
	if (n == 0)
	{
		return;
	}

	/* ============================ Create buttons from the found scene names ============================= */
	m_sceneMenu->ClearButtons();

	inverseN = 1.0f / n;

	for (unsigned int i = 0; i < n; i++)
	{
		// Calculate position and dimensions
		halfDimensions = DirectX::SimpleMath::Vector2(m_spriteFont->MeasureString(wSceneNames[i].c_str())) / 2.0f;
		position.x = halfDimensions.x;
		position.y = 96 + halfDimensions.y * (1 + i * 2);

		m_sceneMenu->CreateButton(
			this,
			&EditorLoadState::LoadScene,
			wSceneNames[i],
			position,
			Vector2i(halfDimensions.x, halfDimensions.y)
		);
	}
}

void EditorLoadState::LoadScene(const std::string & sceneName)
{
	if (!SceneStorage::Get()->LoadScene(sceneName))
	{
		// Send event to leave editor
		EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
		return;
	}

	Scene* scene = SceneStorage::Get()->GetScene(sceneName);

	if (scene == m_scene)
	{
		// This scene is the current one
		return;
	}

	m_scene = scene;
	m_scene->LoadIntoRenderManager();

	// Notify of a scene change, and reset selection
	EventDispatcher::Get()->Emit(Event(EventType::SWITCHED_SCENE, m_scene));
	EventDispatcher::Get()->Emit(Event(EventType::SWITCHED_SELECTED_OBJECT, nullptr));
}