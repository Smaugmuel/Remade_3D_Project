#include "EditorRotateState.hpp"

// For input
#include "Input.hpp"
#include <Windows.h>

// For rotating the object
#include "PlayerCameraManager.hpp"
#include "Camera.hpp"
#include "Frustum.hpp"
#include "TextureObject.hpp"


EditorRotateState::EditorRotateState() : EditorState::EditorState()
{
}

EditorRotateState::~EditorRotateState()
{
}

bool EditorRotateState::Initialize()
{
	if (!EditorState::InitializeIcon("Icons/RotateIcon.png"))
	{
		return false;
	}

	return true;
}

void EditorRotateState::ProcessInput()
{
	Input* input = Input::Get();

	if (input->IsKeyDown(VK_LBUTTON))
	{
		// Rotate the object based on mouse movement

		if (input->IsKeyPressed(VK_LBUTTON))
		{
			// Set the current movement directions

			Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
			Vector3f corners[8];

			FrustumCorners(cam->GetViewMatrix(), cam->GetProjectionMatrix(), corners);
			m_screenDirectionX = (corners[4] - corners[0]).normalized();
			m_screenDirectionY = (corners[2] - corners[0]).normalized();
		}

		if (m_selectedObject)
		{
			// An object is currently chosen

			Vector2f mouseMovement = input->MouseMovement();

			if (mouseMovement != Vector2f(0, 0))
			{
				// Mouse moved

				mouseMovement *= 0.0025f;

				m_selectedObject->RotateGlobal(m_screenDirectionX, mouseMovement.y);
				m_selectedObject->RotateGlobal(m_screenDirectionY, mouseMovement.x);

				//m_selectedObject->SetRotation(MatrixToEuler(m_selectedObject->GetWorldMatrix()));
			}
		}
	}
}

void EditorRotateState::Update(float dt)
{
}

void EditorRotateState::Render()
{
}

void EditorRotateState::RenderHUD()
{
	EditorState::RenderHUD();
}
