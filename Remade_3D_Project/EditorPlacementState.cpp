#include "EditorPlacementState.hpp"

// For input
#include "Input.hpp"
#include <windows.h>

// For placing the preview object
#include "TextureObject.hpp"
#include "PickingRay.hpp"
#include "Scene.hpp"
#include "RenderManager.hpp"

// For rendering the preview object
#include "Direct3D.hpp"
#include "ModelStorage.hpp"
#include "TextureModel.hpp"
#include "ConstantBufferStorage.hpp"
#include "TextureStorage.hpp"
#include "ShaderManager.hpp"

EditorPlacementState::EditorPlacementState() : EditorState::EditorState()
{
}

EditorPlacementState::~EditorPlacementState()
{
	delete m_previewObject;
}

bool EditorPlacementState::Initialize()
{
	m_previewObject = new TextureObject;
	if (!m_previewObject->Initialize("turret.obj", "turret_tex_v3.png"))
		return false;

	if (!EditorState::InitializeIcon("Icons/PlaceIcon.png"))
	{
		return false;
	}

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
	// Position the preview object
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
	// Render the preview object

	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	ID3D11ShaderResourceView* texture = TextureStorage::Get()->GetTexture(m_previewObject->GetTextureName());
	TextureModel* model = ModelStorage::Get()->GetTextureModel(m_previewObject->GetModelName());

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::TEXTURE);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	model->SetupRender(deviceContext);
	ConstantBufferStorage::Get()->SetVSWorldMatrix(deviceContext, m_previewObject->GetWorldMatrix());

	model->Render(deviceContext);
}

void EditorPlacementState::RenderHUD()
{
	EditorState::RenderHUD();
}