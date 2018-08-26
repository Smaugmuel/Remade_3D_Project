#include "EditorRotateState.hpp"

#include "../Engine/Input/Input.hpp"
//#include "../Engine/Camera/PlayerCameraManager.hpp"
#include "../Engine/FrameWork/FrameWork.hpp"
#include "../Engine/Camera/Camera.hpp"
#include "../Engine/Math/Frustum.hpp"
#include "../Engine/Objects/Objects/TextureObject.hpp"

#include <Windows.h>


EditorRotateState::EditorRotateState() : EditorState::EditorState()
{
}

EditorRotateState::~EditorRotateState()
{
}

bool EditorRotateState::Initialize()
{
	/*if (!EditorState::InitializeIcon("Icons/RotateIcon.png"))
	{
		return false;
	}*/

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

			Camera* cam = FrameWork::Get()->GetPlayerCameraManager()->GetCurrentCamera();
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

				if (input->IsKeyDown('X'))
				{
					m_selectedObject->RotateGlobal(Vector3f(1, 0, 0), mouseMovement.x);
				}
				else if (input->IsKeyDown('Y'))
				{
					m_selectedObject->RotateGlobal(Vector3f(0, 1, 0), mouseMovement.x);
				}
				else if (input->IsKeyDown('Z'))
				{
					m_selectedObject->RotateGlobal(Vector3f(0, 0, 1), mouseMovement.x);
				}
				else
				{
					m_selectedObject->RotateGlobal(m_screenDirectionX, mouseMovement.y);
					m_selectedObject->RotateGlobal(m_screenDirectionY, mouseMovement.x);

					//m_selectedObject->SetRotation(MatrixToEuler(m_selectedObject->GetWorldMatrix()));
				}
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

//void EditorRotateState::RenderHUD()
//{
//	EditorState::RenderHUD();
//}
