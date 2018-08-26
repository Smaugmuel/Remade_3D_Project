#include "SceneManagerV3.hpp"
#include "../FrameWork/FrameWorkManager.hpp"
#include "ModelManager.hpp"
#include "MaterialManager.hpp"

SceneManagerV3::SceneManagerV3() :
	m_modelManager(nullptr), m_materialManager(nullptr), m_frameWorkManager(nullptr)
{
}

SceneManagerV3::~SceneManagerV3()
{
}

bool SceneManagerV3::Initialize(ModelManager * modelManager, MaterialManager * materialManager, FrameWorkManager * frameWorkManager)
{
	m_modelManager = modelManager;
	m_materialManager = materialManager;
	m_frameWorkManager = frameWorkManager;

	m_viewProjBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(ViewProjectionMatrices));
	m_worldBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(Matrix));

	if (m_viewProjBufferIndex == -1 || m_worldBufferIndex == -1)
		return false;

	return true;
}

void SceneManagerV3::Render()
{
	/*
	Set up first deferred pass
	*/
	m_frameWorkManager->SetFirstPassRenderTargets();

	/*
	Set up sampler state
	*/
	m_frameWorkManager->GetSamplerManager()->SetSamplerStateToPixelShader(SamplerType::CLAMP, 0);

	/*
	Set up first deferred pass
	*/
	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::FIRST_PASS);

	/*
	Set view- and projection matrices
	*/
	m_frameWorkManager->GetConstantBufferManager()->SetConstantBufferToVertexShader(m_viewProjBufferIndex, 0);

	/*
	Render first deferred pass
	*/
	unsigned int nObj = m_objects.size();
	for (unsigned int i = 0; i < nObj; i++)
	{
		TexturedModel* model = m_modelManager->GetModel(m_objects[i].modelIndex);
		Material* material = m_materialManager->GetMaterial(model->materialIndex);

		/*
		Set vertex buffer
		*/
		m_frameWorkManager->GetVertexBufferManager()->SetBufferToInputAssembler(model->vertexBufferIndex, 0);
		
		/*
		Set texture
		*/
		m_frameWorkManager->GetTextureManager()->SetTextureToPixelShader(material->textureIndex, 0);

		/*
		Set world matrix
		*/
		Matrix world = m_objects[i].worldMatrix.GetTranspose();
		m_frameWorkManager->GetConstantBufferManager()->MapDataToBuffer(m_worldBufferIndex, static_cast<void*>(&world));
		m_frameWorkManager->GetConstantBufferManager()->SetConstantBufferToVertexShader(m_worldBufferIndex, 1);

		/*
		Render object
		*/
		m_frameWorkManager->RenderWithCurrentSettings(model->nrOfVertices);
	}

	/*
	Render second pass of deferred rendering
	*/
	m_frameWorkManager->RenderLightPass();
}

void SceneManagerV3::SetViewAndProjectionMatrices(const Matrix & view, const Matrix & projection)
{
	ViewProjectionMatrices wp(view, projection);

	m_frameWorkManager->GetConstantBufferManager()->MapDataToBuffer(m_viewProjBufferIndex, static_cast<void*>(&wp));
}

int SceneManagerV3::CreateObject()
{
	m_objects.push_back(ObjectV3());
	return m_objects.size() - 1;
}

ObjectV3 * SceneManagerV3::GetObjectV3(int index)
{
	return index >= 0 && index < static_cast<int>(m_objects.size()) ? &m_objects[index] : nullptr;
}