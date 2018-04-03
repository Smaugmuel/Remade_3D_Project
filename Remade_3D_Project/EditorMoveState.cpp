#include "EditorMoveState.hpp"

// For input
#include "Input.hpp"
#include <Windows.h>

// For moving the object
//#include "PickingRay.hpp"
#include "PlayerCameraManager.hpp"
#include "Camera.hpp"
#include "TextureObject.hpp"
#include "Frustum.hpp"

EditorMoveState::EditorMoveState() : EditorState::EditorState(), m_distanceMultiplier(0.0f)
{
}

EditorMoveState::~EditorMoveState()
{
}

bool EditorMoveState::Initialize()
{
	if (!EditorState::InitializeIcon("Icons/MoveIcon.png"))
	{
		return false;
	}

	return true;
}

void EditorMoveState::ProcessInput()
{
	Input* input = Input::Get();
	
	if (input->IsKeyDown(VK_LBUTTON))
	{
		// Move the object based on mouse movement

		if (input->IsKeyPressed(VK_LBUTTON))
		{
			// Set the current movement directions

			Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
			Vector3f corners[8];

			FrustumCorners(cam->GetViewMatrix(), cam->GetProjectionMatrix(), corners);
			m_screenDirectionX = (corners[4] - corners[0]).normalized();
			m_screenDirectionY = (corners[2] - corners[0]).normalized();

			if (m_selectedObject)
			{
				m_distanceMultiplier = (m_selectedObject->GetPosition() - cam->GetPosition()).length();
			}
		}

		if (m_selectedObject)
		{
			// An object is currently chosen

			Vector2f mouseMovement = input->MouseMovement();
			
			if (mouseMovement != Vector2f(0, 0))
			{
				// Mouse moved

				Vector3f movementX = m_screenDirectionX * (mouseMovement.x * 0.0025f);
				Vector3f movementY = m_screenDirectionY * (-mouseMovement.y * 0.0025f);

				m_selectedObject->Move((movementX + movementY) * m_distanceMultiplier);
			}
		}
	}
}

void EditorMoveState::Update(float dt)
{
}

void EditorMoveState::Render()
{
}

void EditorMoveState::RenderHUD()
{
	EditorState::RenderHUD();
}