#include "EditorScaleState.hpp"

// For input
#include "Input.hpp"
#include <Windows.h>

// For changing the object
#include "TextureObject.hpp"

EditorScaleState::EditorScaleState() : EditorState::EditorState()
{
}

EditorScaleState::~EditorScaleState()
{
}

bool EditorScaleState::Initialize()
{
	if (!EditorState::InitializeIcon("Icons/ScaleIcon.png"))
	{
		return false;
	}

	return true;
}

void EditorScaleState::ProcessInput()
{
	Input* input = Input::Get();

	if (input->IsKeyDown(VK_LBUTTON))
	{
		// Rotate the object based on mouse movement

		if (m_selectedObject)
		{
			// An object is currently chosen

			Vector2f mouseMovement = input->MouseMovement();

			if (mouseMovement != Vector2f(0, 0))
			{
				// Mouse moved

				mouseMovement *= 0.0025f;

				m_selectedObject->SetScale(m_selectedObject->GetScale() * (1 + mouseMovement.x));

				//m_selectedObject->SetRotation(MatrixToEuler(m_selectedObject->GetWorldMatrix()));
			}
		}
	}
}

void EditorScaleState::Update(float dt)
{
}

void EditorScaleState::Render()
{
}

void EditorScaleState::RenderHUD()
{
	EditorState::RenderHUD();
}