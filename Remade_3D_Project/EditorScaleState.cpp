#include "EditorScaleState.hpp"

// For input
#include "Input.hpp"
#include <Windows.h>

// For changing the object
#include "TextureObject.hpp"

// For the scale icon
#include "HUDObject.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"

EditorScaleState::EditorScaleState() : EditorState::EditorState(), m_scaleIcon(nullptr)
{
}

EditorScaleState::~EditorScaleState()
{
	delete m_scaleIcon;
}

bool EditorScaleState::Initialize()
{
	m_scaleIcon = new HUDObject;
	if (!m_scaleIcon->Initialize(Direct3D::Get()->GetDevice(), "Icons/ScaleIcon.png", Vector2i(300, 300), Vector2i(32, 32)))
	{
		return false;
	}
	m_scaleIcon->SetPosition(Vector2i(0, 64));
	m_scaleIcon->SetDimensions(Vector2i(32, 32));

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
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	ID3D11ShaderResourceView* iconTexture = m_scaleIcon->GetShaderResourceView();
	deviceContext->PSSetShaderResources(0, 1, &iconTexture);

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::HUD);

	m_scaleIcon->Render(Direct3D::Get()->GetDeviceContext());
}