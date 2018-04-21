#include "EditorSelectionState.hpp"

// For input
#include "Input.hpp"
#include <windows.h>

// For loading the texture
#include "TextureStorage.hpp"
#include "Direct3D.hpp"

// For selecting the cube
#include "PickingRay.hpp"
#include "Collision.hpp"
#include "TextureObject.hpp"
#include "Scene.hpp"
#include "RenderManager.hpp"

#include "EventDispatcher.hpp"

EditorSelectionState::EditorSelectionState() : EditorState::EditorState()
{
}

EditorSelectionState::~EditorSelectionState()
{
}

bool EditorSelectionState::Initialize()
{
	if (!EditorState::InitializeIcon("Icons/SelectIcon.png"))
	{
		return false;
	}

	if (!TextureStorage::Get()->LoadTexture(Direct3D::Get()->GetDevice(), "BrickWallRaw.jpg"))
	{
		return false;
	}

	return true;
}

void EditorSelectionState::ProcessInput()
{
	Input* input = Input::Get();

	if (input->IsKeyPressed(VK_LBUTTON))
	{
		SelectCube(HF::CreatePickingRay());
	}

	if (input->IsKeyPressed(VK_DELETE))
	{
		if (m_selectedObject)
		{
			if (m_scene->RemoveTexturedObject(m_selectedObject))
			{
				m_scene->LoadIntoRenderManager();
				
				m_selectedObject = nullptr;

				EventDispatcher::Get()->Emit(Event(EventType::SWITCHED_SELECTED_OBJECT, static_cast<void*>(m_selectedObject)));
			}
		}
	}
}

void EditorSelectionState::Update(float dt)
{
}

void EditorSelectionState::Render()
{
}

void EditorSelectionState::RenderHUD()
{
	EditorState::RenderHUD();
}

void EditorSelectionState::SelectCube(const Ray& ray)
{
	RenderManager* renderManager = RenderManager::Get();

	OBB obb;
	IntersectionData data;
	
	TextureObject** objects;
	unsigned int n;

	float closestCubeDistance = INFINITY;
	int closestCube = -1;

	// Retrieve objects from scene
	objects = m_scene->GetTexturedObjects();
	n = m_scene->GetNrOfTexturedObjects();

	// Do nothing when there are 0 objects in the scene
	if (n == 0)
	{
		return;
	}

	for (unsigned int i = 0; i < n; i++)
	{
		obb = objects[i]->GetOBB();

		// Check if ray intersects with cube
		data = RayVsOBB(ray.origin, ray.direction, obb);
		if (data.intersection)
		{
			// Check if this intersection is the closest one
			if (data.distance < closestCubeDistance)
			{
				closestCubeDistance = data.distance;
				closestCube = i;
			}
		}

		// Set default texture to un-highlight cube
		if (objects[i]->GetTextureName() != objects[i]->GetDefaultTextureName())
		{
			renderManager->ChangeTexture(objects[i], objects[i]->GetDefaultTextureName());
		}
	}

	// Check if a cube was intersected
	if (closestCube != -1)
	{
		// Select object
		m_selectedObject = objects[closestCube];

		// Change texture to highlight cube 
		if (m_selectedObject->GetTextureName() != "BrickWallRaw.jpg")
		{
			renderManager->ChangeTexture(m_selectedObject, "BrickWallRaw.jpg");
		}
	}
	else
	{
		// Unselect object
		m_selectedObject = nullptr;
	}


	EventDispatcher::Get()->Emit(Event(EventType::SWITCHED_SELECTED_OBJECT, static_cast<void*>(m_selectedObject)));
}