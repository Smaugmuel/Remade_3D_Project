#include "EditorSaveState.hpp"

// For input
#include "Input.hpp"

// For the scene
#include "Scene.hpp"
#include "SceneStorage.hpp"

// For the save icon
#include "HUDObject.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"

// For error management
#include "EventDispatcher.hpp"

EditorSaveState::EditorSaveState() : EditorState::EditorState(), m_saveIcon(nullptr)
{
}

EditorSaveState::~EditorSaveState()
{
	delete m_saveIcon;
}

bool EditorSaveState::Initialize()
{
	m_saveIcon = new HUDObject;
	if (!m_saveIcon->Initialize(Direct3D::Get()->GetDevice(), "Icons/SaveIcon.png", Vector2i(300, 300), Vector2i(32, 32)))
	{
		return false;
	}
	m_saveIcon->SetPosition(Vector2i(0, 64));
	m_saveIcon->SetDimensions(Vector2i(32, 32));

	return true;
}

void EditorSaveState::ProcessInput()
{
	Input* input = Input::Get();

	// Switch, save, load scenes. or switch editor mode
	if (input->IsKeyDown(VK_CONTROL))
	{
		// Change scenes

		/*bool switchedScene = false;

		if (input->IsKeyPressed(48 + 1))
		{
			if (m_scene->GetName() != "Scene1")
			{
				// Load scene
				if (!SceneStorage::Get()->LoadScene("Scene1"))
				{
					EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
					return;
					// Send event to leave editor
				}

				// Set scene
				m_scene = SceneStorage::Get()->GetScene("Scene1");

				switchedScene = true;
			}
		}
		if (input->IsKeyPressed(48 + 2))
		{
			if (m_scene->GetName() != "Scene2")
			{
				if (!SceneStorage::Get()->LoadScene("Scene2"))
				{
					EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
					return;
					// Send event to leave editor
				}

				// Set scene
				m_scene = SceneStorage::Get()->GetScene("Scene2");

				switchedScene = true;
			}
		}
		if (input->IsKeyPressed(48 + 3))
		{
			if (m_scene->GetName() != "Scene3")
			{
				if (!SceneStorage::Get()->LoadScene("Scene3"))
				{
					EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
					return;
					// Send event to leave editor
				}

				// Set scene
				m_scene = SceneStorage::Get()->GetScene("Scene3");

				switchedScene = true;
			}
		}
		if (input->IsKeyPressed(48 + 4))
		{
			if (m_scene->GetName() != "Scene4")
			{
				if (!SceneStorage::Get()->LoadScene("Scene4"))
				{
					EventDispatcher::Get()->Emit(Event(EventType::POP_GAMESTATE));
					return;
					// Send event to leave editor
				}

				// Set scene
				m_scene = SceneStorage::Get()->GetScene("Scene4");

				switchedScene = true;
			}
		}*/

		// Load or save current scene
		if (input->IsKeyPressed('O'))
		{
			m_scene->SaveToFile(m_scene->GetName());
		}

		/*if (switchedScene)
		{
			// Set up rendering of scene
			m_scene->LoadIntoRenderManager();

			// Notify of a scene change, and reset selection
			EventDispatcher::Get()->Emit(Event(EventType::SWITCHED_SCENE, m_scene));
			EventDispatcher::Get()->Emit(Event(EventType::SWITCHED_SELECTED_OBJECT, nullptr));
		}*/
	}
}

void EditorSaveState::Update(float dt)
{
}

void EditorSaveState::Render()
{
}

void EditorSaveState::RenderHUD()
{
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	ID3D11ShaderResourceView* iconTexture = m_saveIcon->GetShaderResourceView();
	deviceContext->PSSetShaderResources(0, 1, &iconTexture);

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::HUD);

	m_saveIcon->Render(Direct3D::Get()->GetDeviceContext());
}