#include "EditorPlacementState.hpp"

//#include "../Engine/Input/Input.hpp"

#include "../Engine/Objects/Materials/Material.hpp"
//#include "../Engine/Objects/Materials/MaterialStorage.hpp"
#include "../Engine/Objects/Materials/MaterialStorageV2.hpp"
#include "../Engine/Objects/Objects/TextureObject.hpp"
#include "../Engine/Objects/Models/TextureModel.hpp"
//#include "../Engine/Objects/Models/ModelStorage.hpp"
#include "../Engine/Objects/Models/ModelStorageV2.hpp"
//#include "../Engine/Objects/Textures/TextureStorage.hpp"
#include "../Engine/Objects/Textures/TextureStorageV2.hpp"

#include "../Engine/Render/RenderManager.hpp"
//#include "../Engine/FrameWork/Direct3D.hpp"
//#include "../Engine/Core/Engine.hpp"
//#include "../Engine/FrameWork/FrameWork.hpp"
#include "../Engine/Render/Shaders/ShaderManager.hpp"

#include "../Engine/Math/PickingRay.hpp"

#include "../Engine/Buffers/ConstantBufferStorage.hpp"

#include "Scene.hpp"
#include <windows.h>

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

	// Default the preview object to the cube
	if (!m_previewObject->Initialize("cube_uv.obj"/*, "turret_tex_v3.png"*/))
		return false;

	/*if (!EditorState::InitializeIcon("Icons/PlaceIcon.png"))
	{
		return false;
	}*/

	return true;
}

void EditorPlacementState::ProcessInput()
{
	/*Input* input = Input::Get();

	if (input->IsKeyPressed(VK_RBUTTON))
	{
		ModelStorageV2* models = ModelStorageV2::Get();
		MaterialStorageV2* materials = MaterialStorageV2::Get();
		TextureStorageV2* textures = TextureStorageV2::Get();

		unsigned int nrOfTextureModels = models->GetNrOfTextureModels();
		
		static int modelIndex = 0;
		modelIndex = (modelIndex + 1) % nrOfTextureModels;

		m_previewObject->SetModel(modelIndex);
		m_previewObject->SetTexture(materials->GetMaterial(models->GetTextureModel(modelIndex)->GetMaterialIndex())->GetTextureIndex());
	}

	// Create a new object at preview object
	if (input->IsKeyPressed(VK_LBUTTON))
	{
		m_scene->AddTexturedObject(
			m_previewObject->GetModelName(),
			//m_previewObject->GetTextureName(),
			m_previewObject->GetPosition(),
			m_previewObject->GetRotation(),
			m_previewObject->GetScale()
		);

		RenderManager::Get()->AddTexturedObject(m_scene->GetTexturedObjects()[m_scene->GetNrOfTexturedObjects() - 1]);
	}*/
}

void EditorPlacementState::Update(float dt)
{
	// Position the preview object
	/*if (m_previewObject)
	{
		Ray ray = HF::CreatePickingRay(FrameWork::Get()->GetPlayerCameraManager()->GetCurrentCamera());
		Vector3f position = ray.origin + ray.direction * 10;

		m_previewObject->SetPosition(position);
		m_previewObject->Update();
	}*/
}

void EditorPlacementState::Render()
{
	// Render the preview object

	/*ID3D11DeviceContext* deviceContext = FrameWork::Get()->GetDirect3D()->GetDeviceContext();
	//ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	ID3D11ShaderResourceView* texture = TextureStorageV2::Get()->GetTexture(m_previewObject->GetTextureIndex());
	TextureModel* model = ModelStorageV2::Get()->GetTextureModel(m_previewObject->GetModelIndex());

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::TEXTURE);
	deviceContext->PSSetShaderResources(0, 1, &texture);
	model->SetupRender();
	ConstantBufferStorage::Get()->SetVSWorldMatrix(deviceContext, m_previewObject->GetWorldMatrix());

	model->Render();*/
}

//void EditorPlacementState::RenderHUD()
//{
//	EditorState::RenderHUD();
//}