#include "EditorSelectionState.hpp"

// For input
#include "Input.hpp"
#include <windows.h>

// For creating the picking ray
#include "PickingRay.hpp"

// For checking the ray against objects
#include "Collision.hpp"
#include "TextureObject.hpp"
#include "Scene.hpp"

// For changing the objects
#include "RenderManager.hpp"

// For the select icon
#include "HUDObject.hpp"
#include "Direct3D.hpp"
#include "TextureStorage.hpp"
#include "ShaderManager.hpp"

#include "EventDispatcher.hpp"

EditorSelectionState::EditorSelectionState() : EditorState::EditorState(), m_selectIcon(nullptr)
{
}

EditorSelectionState::~EditorSelectionState()
{
	delete m_selectIcon;
}

bool EditorSelectionState::Initialize()
{
	m_selectedObject = nullptr;

	m_selectIcon = new HUDObject;
	if (!m_selectIcon->Initialize(Direct3D::Get()->GetDevice(), "Icons/SelectIcon.png", Vector2i(300, 300), Vector2i(32, 32)))
	{
		return false;
	}
	m_selectIcon->SetPosition(Vector2i(0, 64));
	m_selectIcon->SetDimensions(Vector2i(32, 32));

	return true;
}

void EditorSelectionState::ProcessInput()
{
	Input* input = Input::Get();

	if (input->IsKeyPressed(VK_LBUTTON))
	{
		SelectCube(HF::CreatePickingRay());
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
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	ID3D11ShaderResourceView* iconTexture = m_selectIcon->GetShaderResourceView();
	deviceContext->PSSetShaderResources(0, 1, &iconTexture);

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::HUD);

	m_selectIcon->Render(deviceContext);
}

void EditorSelectionState::SelectCube(const Ray& ray)
{
	RenderManager* renderManager = RenderManager::Get();

	OBB obb;
	Sphere sphere;
	IntersectionData data;
	
	TextureObject** objects;
	unsigned int n;

	float closestCubeDistance = INFINITY;
	int closestCube = -1;


	// Create a default OBB (AABB for now)
	obb.halfSides[0] = 1;
	obb.halfSides[1] = 1;
	obb.halfSides[2] = 1;
	obb.vectors[0] = Vector3f(1, 0, 0);
	obb.vectors[1] = Vector3f(0, 1, 0);
	obb.vectors[2] = Vector3f(0, 0, 1);

	// Create a sphere around OBB
	sphere.radius = 0.0f;	// False radius, but it is unused
	sphere.radius2 = obb.halfSides[0] * obb.halfSides[0] + obb.halfSides[1] * obb.halfSides[1] + obb.halfSides[2] * obb.halfSides[2];

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
		sphere.center = objects[i]->GetPosition();
		
		// Check if ray intersects with sphere around cube
		data = RayVSSphere(ray.origin, ray.direction, sphere);
		if (data.intersection)
		{
			obb.center = sphere.center;
			
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

				//// Change texture to highlight cube
				//if (objects[i]->GetTextureName() != "BrickWallRaw.jpg")
				//{
				//	renderManager->ChangeTexture(objects[i], "BrickWallRaw.jpg");
				//}
				//continue;
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