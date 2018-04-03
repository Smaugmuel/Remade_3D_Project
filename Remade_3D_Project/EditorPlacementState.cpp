#include "EditorPlacementState.hpp"

// For input
#include "Input.hpp"
#include <windows.h>

// For creating the picking ray
#include "PickingRay.hpp"

// For managing the preview object
#include "TextureObject.hpp"
#include "Scene.hpp"

// For changing the preview object
#include "RenderManager.hpp"

// For rendering the preview object
#include <d3d11.h>
#include "Direct3D.hpp"
#include "ModelStorage.hpp"
#include "TextureStorage.hpp"
#include "ShaderStorage.hpp"
#include "ShaderManager.hpp"
#include "ConstantBufferStorage.hpp"
#include "TextureModel.hpp"

// For the place icon
#include "HUDObject.hpp"
#include "Direct3D.hpp"
#include "ShaderManager.hpp"

EditorPlacementState::EditorPlacementState() : EditorState::EditorState(), m_placeIcon(nullptr)
{
}

EditorPlacementState::~EditorPlacementState()
{
	delete m_previewObject;
	delete m_placeIcon;
}

bool EditorPlacementState::Initialize()
{
	/* ======================================= Preview object =============================================== */
	m_previewObject = new TextureObject;
	if (!m_previewObject->Initialize("turret.obj", "turret_tex_v3.png"))
		return false;

	/* ======================================= Place icon =============================================== */
	m_placeIcon = new HUDObject;
	if (!m_placeIcon->Initialize(Direct3D::Get()->GetDevice(), "Icons/PlaceIcon.png", Vector2i(300, 300), Vector2i(32, 32)))
	{
		return false;
	}
	m_placeIcon->SetPosition(Vector2i(0, 64));
	m_placeIcon->SetDimensions(Vector2i(32, 32));

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
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	TextureStorage* textureStorage = TextureStorage::Get();
	ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::TEXTURE);

	// Set up texture
	ID3D11ShaderResourceView* texture = TextureStorage::Get()->GetTexture(m_previewObject->GetTextureName());
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// Set up model
	TextureModel* model = modelStorage->GetTextureModel(m_previewObject->GetModelName());
	model->SetupRender(deviceContext);

	// Set up world matrix
	bufferStorage->SetVSWorldMatrix(deviceContext, m_previewObject->GetWorldMatrix());

	// Render
	model->Render(deviceContext);
}

void EditorPlacementState::RenderHUD()
{
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	ID3D11ShaderResourceView* iconTexture = m_placeIcon->GetShaderResourceView();
	deviceContext->PSSetShaderResources(0, 1, &iconTexture);

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::HUD);

	m_placeIcon->Render(deviceContext);
}