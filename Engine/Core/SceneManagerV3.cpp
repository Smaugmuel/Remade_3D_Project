#include "SceneManagerV3.hpp"
#include "../FrameWork/FrameWorkManager.hpp"
#include "ModelManager.hpp"
#include "MaterialManager.hpp"

SceneManagerV3::SceneManagerV3() :
	m_objects(nullptr), m_modelManager(nullptr), m_materialManager(nullptr), m_frameWorkManager(nullptr)
{
}

SceneManagerV3::~SceneManagerV3()
{
	if (m_objects)
	{
		delete[] m_objects;
	}
}

bool SceneManagerV3::Initialize(ModelManager * modelManager, MaterialManager * materialManager, FrameWorkManager * frameWorkManager)
{
	m_objects = new ObjectV3[MAX_NR_OF_OBJECTS];
	if (!m_objects)
		return false;

	m_modelManager = modelManager;
	m_materialManager = materialManager;
	m_frameWorkManager = frameWorkManager;

	m_nrOfObjects = 0;

	/*
	Initialize lights
	*/
	LightBuffer lightBuffer;
	lightBuffer.nrOfLights = min(max(4, 0), MAX_NR_OF_LIGHTS);
	for (int i = 0; i < lightBuffer.nrOfLights; i++)
	{
		lightBuffer.lights[i].position = Vector3f((1 - i % 2) * 100.0f, 10.0f, (1 - i / 2) * 100.0f);
		lightBuffer.lights[i].dropoff = -0.01f * (i + 1);
	}

	m_viewProjBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(Matrix));
	m_worldBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(Matrix));
	m_lightBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(LightBuffer));

	if (m_viewProjBufferIndex == -1 || m_worldBufferIndex == -1 || m_lightBufferIndex == -1)
		return false;

	m_frameWorkManager->GetConstantBufferManager()->MapDataToBuffer(m_lightBufferIndex, &lightBuffer);

	/*
	For instancing
	*/
#ifdef USING_INSTANCING
	m_matrixBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(MatrixBuffer));
	m_instanceBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(InstanceBuffer));

	if (m_matrixBufferIndex == -1 || m_instanceBufferIndex == -1)
		return false;
#endif

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
	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::GEOMETRY_PASS);

	/*
	Set view- and projection matrices
	*/
	m_frameWorkManager->GetConstantBufferManager()->SetConstantBufferToVertexShader(m_viewProjBufferIndex, 0);

	//unsigned int nObj = m_objects.size();
	//if (nObj == 0)
	if (m_nrOfObjects == 0)
	{
		return;
	}

	TexturedModel* model = m_modelManager->GetModel(m_objects[0].modelIndex);
	Material* material = m_materialManager->GetMaterial(model->materialIndex);
	
	/*
	Set model-specific data
	*/
	m_frameWorkManager->GetVertexBufferManager()->SetBufferToInputAssembler(model->vertexBufferIndex, 0);
	m_frameWorkManager->GetTextureManager()->SetTextureToPixelShader(material->textureIndex, 0);

	/*
	Set object-specific data
	Render first deferred pass
	*/
	for (unsigned int i = 0; i < m_nrOfObjects; i++)
	{
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
	Set lights
	*/
	m_frameWorkManager->GetConstantBufferManager()->SetConstantBufferToPixelShader(m_lightBufferIndex, 0);

	/*
	Render second pass of deferred rendering
	*/
	m_frameWorkManager->RenderLightPass();
}

void SceneManagerV3::SetViewAndProjectionMatrices(const Matrix & view, const Matrix & projection)
{
	//ViewProjectionMatrices vp(view.GetTranspose(), projection.GetTranspose());
	Matrix vp = projection.GetTranspose() * view.GetTranspose();

	m_frameWorkManager->GetConstantBufferManager()->MapDataToBuffer(m_viewProjBufferIndex, static_cast<void*>(&vp));
}

int SceneManagerV3::CreateObject()
{
	/*
	Simple "solution" for now
	*/
	if (m_nrOfObjects == MAX_NR_OF_OBJECTS)
	{
		return -1;
	}

	return m_nrOfObjects++;
}

ObjectV3 * SceneManagerV3::GetObjectV3(int index)
{
	return index >= 0 && index < static_cast<int>(m_nrOfObjects) ? &m_objects[index] : nullptr;
}

unsigned int SceneManagerV3::GetNrOfObjects() const
{
	return m_nrOfObjects;
}
