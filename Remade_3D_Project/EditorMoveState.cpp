#include "EditorMoveState.hpp"

// For input
#include "Input.hpp"
#include <Windows.h>

// For calculating directions
//#include "PickingRay.hpp"
#include "PlayerCameraManager.hpp"
#include "Camera.hpp"
#include "Frustum.hpp"

// For changing the object
#include "TextureObject.hpp"

// For the move icon
#include "HUDObject.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"

EditorMoveState::EditorMoveState() : EditorState::EditorState(), m_moveIcon(nullptr), m_distanceMultiplier(0.0f)
{
}

EditorMoveState::~EditorMoveState()
{
	delete m_moveIcon;
}

bool EditorMoveState::Initialize()
{
	m_moveIcon = new HUDObject;
	if (!m_moveIcon->Initialize(Direct3D::Get()->GetDevice(), "Icons/MoveIcon.png", Vector2i(300, 300), Vector2i(32, 32)))
	{
		return false;
	}
	m_moveIcon->SetPosition(Vector2i(0, 64));
	m_moveIcon->SetDimensions(Vector2i(32, 32));

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
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	ID3D11ShaderResourceView* iconTexture = m_moveIcon->GetShaderResourceView();
	deviceContext->PSSetShaderResources(0, 1, &iconTexture);

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::HUD);

	m_moveIcon->Render(Direct3D::Get()->GetDeviceContext());
}