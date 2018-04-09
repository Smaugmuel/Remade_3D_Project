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

// For collision
#include "Collision.hpp"
#include "Scene.hpp"

// For displaying collision
#include "RenderManager.hpp"

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
				mouseMovement *= 0.0025f;

				Vector3f movement;

				if (input->IsKeyDown('X'))
				{
					movement = Vector3f(1, 0, 0) * mouseMovement.x;
				}
				else if (input->IsKeyDown('Y'))
				{
					movement = Vector3f(0, 1, 0) * mouseMovement.x;
				}
				else if (input->IsKeyDown('Z'))
				{
					movement = Vector3f(0, 0, 1) * mouseMovement.x;
				}
				else
				{
					Vector3f movementAlongScreenX = m_screenDirectionX * mouseMovement.x;
					Vector3f movementAlongScreenY = m_screenDirectionY * -mouseMovement.y;

					movement = movementAlongScreenX + movementAlongScreenY;
				}

				m_selectedObject->Move(movement * m_distanceMultiplier);
			}
		}
	}
}

void EditorMoveState::Update(float dt)
{
	CheckCollisionsOfSelectedCube();
}

void EditorMoveState::Render()
{
}

void EditorMoveState::RenderHUD()
{
	EditorState::RenderHUD();
}

void EditorMoveState::CheckCollisionsOfSelectedCube()
{
	unsigned int nrOfObjects = m_scene->GetNrOfTexturedObjects();
	TextureObject** objects = m_scene->GetTexturedObjects();
	RenderManager* renderManager = RenderManager::Get();
	Collision* collision = Collision::Get();
	OBB obbOfSelectedObject;

	// Ensure that a selected object exists
	if (m_selectedObject)
	{
		obbOfSelectedObject = m_selectedObject->GetOBB();
	}

	// Here could be a return in an else-statement
	// However, I would rather have a fixable crash here if something wrong occurs than not knowing about it

	for (unsigned int i = 0; i < nrOfObjects; i++)
	{
		// Don't check collision with itself
		if (objects[i] == m_selectedObject)
		{
			continue;
		}

		// Check if the objects collide
		if (collision->OBBVSOBB(obbOfSelectedObject, objects[i]->GetOBB()))
		{
			// Check if the object is already highlighted, to avoid changing the render manager every frame
			if (objects[i]->GetTextureName() != "BrickWallRaw.jpg")
			{
				// Highlight object
				renderManager->ChangeTexture(objects[i], "BrickWallRaw.jpg");
			}
		}
		else
		{
			// Check if the object is already un-highlighted, to avoid changing the render manager every frame
			if (objects[i]->GetTextureName() == "BrickWallRaw.jpg")
			{
				// Un-highlight object
				renderManager->ChangeTexture(objects[i], objects[i]->GetDefaultTextureName());
			}
		}
	}
}
