#include "EditorSaveState.hpp"

// For input
#include "../Engine/Framework/Input.hpp"
#include <Windows.h>

// For the scene
#include "Scene.hpp"


EditorSaveState::EditorSaveState() : EditorState::EditorState()
{
}

EditorSaveState::~EditorSaveState()
{
}

bool EditorSaveState::Initialize()
{
	/*if (!EditorState::InitializeIcon("Icons/SaveIcon.png"))
	{
		return false;
	}*/

	return true;
}

void EditorSaveState::ProcessInput()
{
	/*Input* input = Input::Get();

	if (input->IsKeyDown(VK_CONTROL))
	{
		// Save current scene
		if (input->IsKeyPressed('O'))
		{
			m_scene->SaveToFile(m_scene->GetName());
		}
	}*/
}

void EditorSaveState::Update(float dt)
{
}

void EditorSaveState::Render()
{
}

//void EditorSaveState::RenderHUD()
//{
//	EditorState::RenderHUD();
//}