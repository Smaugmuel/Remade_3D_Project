#include "EditorMoveState.hpp"

// For input
#include "Input.hpp"
#include <Windows.h>

// For picking
#include "PickingRay.hpp"
#include "PlayerCameraManager.hpp"
#include "Camera.hpp"

// For changing the object
#include "TextureObject.hpp"

EditorMoveState::EditorMoveState(StateMachine<EditorState>* stateMachine) :
	EditorState::EditorState(stateMachine)
{
}

EditorMoveState::~EditorMoveState()
{
}

bool EditorMoveState::Initialize()
{
	return true;
}

void EditorMoveState::ProcessInput()
{
	Input* input = Input::Get();
	
	m_mouseLeftDown = input->IsKeyDown(VK_LBUTTON);
	if (input->IsKeyPressed(VK_LBUTTON))
	{
		if (m_selectedObject)
		{
			Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
			m_objectDistance = (m_selectedObject->GetPosition() - cam->GetPosition()).length();
		}
	}
	if (input->IsKeyDown(VK_DOWN))
	{
		m_objectDistance *= 0.99f;
	}
	if (input->IsKeyDown(VK_UP))
	{
		m_objectDistance *= 1.01f;
	}
}

void EditorMoveState::Update(float dt)
{
	if (m_mouseLeftDown)
	{
		if (m_selectedObject)
		{
			Ray ray = HF::CreatePickingRay();

			m_selectedObject->SetPosition(ray.origin + ray.direction * m_objectDistance);
		}
	}
}

void EditorMoveState::Render()
{
}