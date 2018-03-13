#include "EditorPlacementState.hpp"

// For input
#include "Input.hpp"
#include <windows.h>

// For creating the picking ray
#include "PickingRay.hpp"

// For managing the object
#include "TextureObject.hpp"
#include "Scene.hpp"

// For changing the object
#include "RenderManager.hpp"

EditorPlacementState::EditorPlacementState(StateMachine<EditorState>* stateMachine, Scene* scene) :
	EditorState::EditorState(stateMachine), m_scene(scene)
{
}

EditorPlacementState::~EditorPlacementState()
{
	delete m_previewObject;
}

bool EditorPlacementState::Initialize()
{
	/* ======================================= Preview object =============================================== */
	m_previewObject = new TextureObject;
	if (!m_previewObject->Initialize("turret.obj", "turret_tex_v3.png"))
		return false;

	RenderManager::Get()->AddTexturedObject(m_previewObject);

	return true;
}

void EditorPlacementState::ProcessInput()
{
	Input* input = Input::Get();

	// Create a new object at preview object
	if (input->IsKeyPressed(VK_LBUTTON))
	{
		m_scene->AddTexturedObject(
			m_previewObject->GetModelName(),
			m_previewObject->GetTextureName(),
			m_previewObject->GetPosition(),
			m_previewObject->GetRotation(),
			m_previewObject->GetScale()
		);

		RenderManager::Get()->AddTexturedObject(m_scene->GetTexturedObjects()[m_scene->GetNrOfTexturedObjects() - 1]);
	}
}

void EditorPlacementState::Update(float dt)
{
	if (m_previewObject)
	{
		Ray ray = HF::CreatePickingRay();
		Vector3f position = ray.origin + ray.direction * 10;

		m_previewObject->SetPosition(position);
		m_previewObject->Update();
	}
}

void EditorPlacementState::Render()
{
}