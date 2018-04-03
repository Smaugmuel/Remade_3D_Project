#include "EditorRotateState.hpp"

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

// For the rotate icon
#include "HUDObject.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"


EditorRotateState::EditorRotateState() : EditorState::EditorState(), m_rotateIcon(nullptr)
{
}

EditorRotateState::~EditorRotateState()
{
	delete m_rotateIcon;
}

bool EditorRotateState::Initialize()
{
	m_rotateIcon = new HUDObject;
	if (!m_rotateIcon->Initialize(Direct3D::Get()->GetDevice(), "Icons/RotateIcon.png", Vector2i(300, 300), Vector2i(32, 32)))
	{
		return false;
	}
	m_rotateIcon->SetPosition(Vector2i(0, 64));
	m_rotateIcon->SetDimensions(Vector2i(32, 32));

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
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	ID3D11ShaderResourceView* iconTexture = m_rotateIcon->GetShaderResourceView();
	deviceContext->PSSetShaderResources(0, 1, &iconTexture);

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::HUD);

	m_rotateIcon->Render(Direct3D::Get()->GetDeviceContext());
}
