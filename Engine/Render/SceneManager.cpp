#include "SceneManager.hpp"
//#include "../FrameWork/Direct3D.hpp"
//#include "../Core/Engine.hpp"
//#include "../FrameWork/FrameWork.hpp"
#include "../Render/Shaders/ShaderManager.hpp"
//#include "../Objects/Models/ModelStorage.hpp"
//#include "../Objects/Textures/TextureStorage.hpp"
//#include "../Objects/Models/ModelStorageV2.hpp"
//#include "../Objects/Textures/TextureStorageV2.hpp"
#include "../Buffers/ConstantBufferStorage.hpp"
#include "../Objects/Models/TextureModel.hpp"

//#include "../Camera/PlayerCameraManager.hpp"
#include "../Camera/Camera.hpp"

//#include "../Render/SamplerStorage.hpp"

//#include "../Render/DeferredScreenTarget.hpp"

//SceneManager* Singleton<SceneManager>::s_instance = nullptr;

SceneManager::SceneManager() : m_renderMode(RenderModes::NORMAL)
{
}
SceneManager::~SceneManager()
{
}

bool SceneManager::Initialize(FrameWork * frameWork)
{
	m_frameWork = frameWork;
	/*m_vsViewMatID = m_frameWork->CreateConstantBuffer(sizeof(float) * 16);
	m_vsProjMatID = m_frameWork->CreateConstantBuffer(sizeof(float) * 16);*/
	return true;
}

void SceneManager::Update(float dt)
{
	unsigned int n = m_objects.size();
	for (unsigned int i = 0; i < n; i++)
	{
		m_objects[i].object.Update();
	}
}

int SceneManager::AddObject(const std::string & modelName, Vector3f position, Vector3f rotation, Vector3f scale)
{
	SceneObject obj;
	obj.id = m_objects.size();
	if (!obj.object.Initialize(modelName))
	{
		return -1;
	}
	obj.object.SetPosition(position);
	obj.object.SetRotation(rotation);
	obj.object.SetScale(scale);

	m_objects.push_back(obj);

	return obj.id;
}

void SceneManager::SetObjectModel(int id, const std::string & modelName)
{
	switch (int index = GetIndexOfID(id))
	{
	case -1:
		break;
	default:
		m_objects[index].object.SetModelName(modelName);
		break;
	}
}
void SceneManager::SetObjectTexture(int id, const std::string & textureName)
{
	switch (int index = GetIndexOfID(id))
	{
	case -1:
		break;
	default:
		m_objects[index].object.SetTextureName(textureName);
		break;
	}
}
void SceneManager::SetObjectPosition(int id, Vector3f position)
{
	switch (int index = GetIndexOfID(id))
	{
	case -1:
		break;
	default:
		m_objects[index].object.SetPosition(position);
		break;
	}
}
void SceneManager::SetObjectRotation(int id, Vector3f rotation)
{
	switch (int index = GetIndexOfID(id))
	{
	case -1:
		break;
	default:
		m_objects[index].object.SetRotation(rotation);
		break;
	}
}
void SceneManager::SetObjectScale(int id, Vector3f scale)
{
	switch (int index = GetIndexOfID(id))
	{
	case -1:
		break;
	default:
		m_objects[index].object.SetScale(scale);
		break;
	}
}

void SceneManager::SetRenderMode(RenderModes mode)
{
	m_renderMode = mode;
}

void SceneManager::Render()
{
	switch (m_renderMode)
	{
	case RenderModes::NORMAL:
		RenderNormal();
		break;
	case RenderModes::DEFERRED_WITH_SHADOWS:
		RenderDeferredFirstPass();
		RenderShadowPass();
		RenderDeferredLightShadowPass();
		break;
	case RenderModes::DEFERRED_IN_CHUNKS:
		RenderDeferredFirstPassChunks();
		RenderShadowPassChunks();
		RenderDeferredLightShadowPass();
		break;
	case RenderModes::DEFERRED_SPLIT_TEXTURES:
		RenderDeferredFirstPass();
		RenderShadowPass();
		RenderDeferredLightSplitPass();
		break;
	case RenderModes::DEFERRED_WITH_MULTIPLE_LIGHTS:
		RenderDeferredFirstPass();
		RenderDeferredLightMultiplePass();
		break;
	case RenderModes::DEFERRED_WITH_MULTIPLE_LIGHTS_GIVING_SHADOW:
		RenderDeferredFirstPass();
		RenderMultipleShadowsPass();
		RenderDeferredLightMultipleShadowsPass();
		break;
	case RenderModes::DEPTH:
		RenderDepth();
		break;
	default:
		break;
	}
}

void SceneManager::RenderNormal()
{
	/*ID3D11DeviceContext* deviceContext = m_frameWork->GetDirect3D()->GetDeviceContext();
	//ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();

	//TextureStorage* textureStorage = TextureStorage::Get();
	//ModelStorage* modelStorage = ModelStorage::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureStorageV2* textureStorageV2 = TextureStorageV2::Get();
	ModelStorageV2* modelStorageV2 = ModelStorageV2::Get();

	//d3d->SetDefaultTarget();
	//d3d->ClearDefaultTarget();
	//d3d->SetDefaultBlendState();
	//d3d->EnableZBuffer();
	//d3d->SetDefaultRasterizerState();

	// This has no special functionality, but is required for some reason
	// Without changing to an existing shader before the one below, the GUI and scene are not rendered correctly
	// Note that simply setting a nullptr to each shader did not work, nor did a reset of the one below
	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::SINGLE_COLOR);


	Camera* camera = ->Get->GetPlayerCameraManager()->GetCamera(0);
	if (!camera)
	{
		return;
	}

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::TEXTURE);


	bufferStorage->SetVSPointLight(deviceContext, camera->GetPosition(), 1.0f);

	
	bufferStorage->SetVSViewMatrix(deviceContext, camera->GetViewMatrix());
	bufferStorage->SetVSProjectionMatrix(deviceContext, camera->GetProjectionMatrix());

	//ID3D11SamplerState* sampler = SamplerStorage::Get()->GetSampler("PointClamp");
	//deviceContext->PSSetSamplers(0, 1, &sampler);
	
	int previousModel = -1;
	int previousTexture = -1;
	TextureModel* model = nullptr;
	ID3D11ShaderResourceView* texture = nullptr;


	unsigned int n = m_objects.size();
	for (unsigned int i = 0; i < n; i++)
	{
		TextureObject* object = &m_objects[i].object;

		// Set the texture unless it was already set
		if (object->GetTextureIndex() != previousTexture)
		{
			texture = textureStorageV2->GetTexture(object->GetTextureIndex());
			//ID3D11ShaderResourceView* texture = textureStorage->GetTexture(object->GetTextureName());
			deviceContext->PSSetShaderResources(0, 1, &texture);

			previousTexture = object->GetTextureIndex();
		}
		
		// Set the model unless it was already set
		if (object->GetModelIndex() != previousModel)
		{
			model = modelStorageV2->GetTextureModel(object->GetModelIndex());
			//TextureModel* model = modelStorage->GetTextureModel(object->GetModelName());
			model->SetupRender();

			previousModel = object->GetModelIndex();
		}

		// Set the matrix
		bufferStorage->SetVSWorldMatrix(deviceContext, object->GetWorldMatrix());

		// Render
		model->Render();
	}*/
}
void SceneManager::RenderDeferredFirstPass()
{
}
void SceneManager::RenderDeferredFirstPassChunks()
{
	/*Direct3D* d3d = FrameWork::Get()->GetDirect3D();
	//Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaderManager = ShaderManager::Get();
	Camera* cam = FrameWork::Get()->GetPlayerCameraManager()->GetCurrentCamera();

	ModelStorageV2* modelStorage = ModelStorageV2::Get();
	TextureStorageV2* textureStorage = TextureStorageV2::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	TextureModel* model;
	ID3D11ShaderResourceView* texture;

	const unsigned int CHUNK_SIZE = 28;
	DirectX::XMMATRIX worldMatrices[CHUNK_SIZE];


	d3d->SetDeferredTargets();
	d3d->ClearDeferredTargets();

	bufferStorage->SetGSViewMatrix(deviceContext, cam->GetViewMatrix());
	bufferStorage->SetGSProjectionMatrix(deviceContext, cam->GetProjectionMatrix());

	shaderManager->SetShaderType(deviceContext, ShaderType::D_TEXTURE_CHUNK);


	model = modelStorage->GetTextureModel(m_objects[0].object.GetModelIndex());
	model->SetupRender();

	texture = textureStorage->GetTexture(m_objects[0].object.GetTextureIndex());
	shaderManager->SetPerObjectDeferredTextureChunkShaderGroup(deviceContext, texture);

	// Calculate how many render passes are needed
	unsigned int nrOfFullRenderPasses = m_objects.size() / CHUNK_SIZE;
	unsigned int nrOfCubesInFullRenderPasses = nrOfFullRenderPasses * CHUNK_SIZE;
	unsigned int nrOfRemainingCubes = m_objects.size() - nrOfCubesInFullRenderPasses;

	// Render the full chunks of objects
	for (unsigned int i = 0; i < nrOfFullRenderPasses; i++)
	{
		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
		{
			DirectX::XMMATRIX temp = m_objects[i * CHUNK_SIZE + j].object.GetWorldMatrix();
			memcpy(&worldMatrices[j], &temp, sizeof(temp));
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, CHUNK_SIZE);
		model->Render();
	}

	// Render the leftovers
	if (nrOfRemainingCubes > 0)
	{
		for (unsigned int i = 0; i < nrOfRemainingCubes; i++)
		{
			DirectX::XMMATRIX temp = m_objects[nrOfCubesInFullRenderPasses + i].object.GetWorldMatrix();
			memcpy(&worldMatrices[i], &temp, sizeof(temp));
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, nrOfRemainingCubes);
		model->Render();
	}*/
}
void SceneManager::RenderDepth()
{
}
void SceneManager::RenderShadowPass()
{
}
void SceneManager::RenderShadowPassChunks()
{
	/*Direct3D* d3d = FrameWork::Get()->GetDirect3D();
	//Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = FrameWork::Get()->GetPlayerCameraManager()->GetCamera(0);

	ModelStorageV2* modelStorage = ModelStorageV2::Get();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
	TextureModel* model;

	const unsigned int CHUNK_SIZE = 28;
	DirectX::XMMATRIX worldMatrices[CHUNK_SIZE];


	d3d->SetShadowTarget();
	d3d->ClearShadowTarget();

	bufferStorage->SetGSViewMatrix(deviceContext, cam0->GetViewMatrix());
	bufferStorage->SetGSProjectionMatrix(deviceContext, cam0->GetProjectionMatrix());

	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE_CHUNK);
	shaders->SetShaderType(deviceContext, ShaderType::D_SHADOW);


	model = modelStorage->GetTextureModel(m_objects[0].object.GetModelIndex());
	model->SetupRender();

	// Calculate how many render passes are needed
	unsigned int nrOfFullRenderPasses = m_objects.size() / CHUNK_SIZE;
	unsigned int nrOfCubesInFullRenderPasses = nrOfFullRenderPasses * CHUNK_SIZE;
	unsigned int nrOfRemainingCubes = m_objects.size() - nrOfCubesInFullRenderPasses;

	// Render the full chunks of objects
	for (unsigned int i = 0; i < nrOfFullRenderPasses; i++)
	{
		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
		{
			DirectX::XMMATRIX temp = m_objects[i * CHUNK_SIZE + j].object.GetWorldMatrix();
			memcpy(&worldMatrices[j], &temp, sizeof(temp));
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, CHUNK_SIZE);
		model->Render();
	}

	// Render the leftovers
	if (nrOfRemainingCubes > 0)
	{
		for (unsigned int i = 0; i < nrOfRemainingCubes; i++)
		{
			DirectX::XMMATRIX temp = m_objects[nrOfCubesInFullRenderPasses + i].object.GetWorldMatrix();
			memcpy(&worldMatrices[i], &temp, sizeof(temp));
		}

		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
		bufferStorage->SetGSNrOfObjects(deviceContext, nrOfRemainingCubes);
		model->Render();
	}*/
}
void SceneManager::RenderMultipleShadowsPass()
{
}
void SceneManager::RenderDeferredLightShadowPass()
{
	/*Direct3D* d3d = FrameWork::Get()->GetDirect3D();
	//Direct3D* d3d = Direct3D::Get();
	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
	ShaderManager* shaders = ShaderManager::Get();
	Camera* cam0 = FrameWork::Get()->GetPlayerCameraManager()->GetCamera(0);

	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	d3d->SetDefaultTarget();

	shaders->SetShaderType(deviceContext, ShaderType::D_LIGHT);

	bufferStorage->SetPSViewMatrix(deviceContext, cam0->GetViewMatrix());
	bufferStorage->SetPSProjectionMatrix(deviceContext, cam0->GetProjectionMatrix());
	bufferStorage->SetPSPointLight(deviceContext, cam0->GetPosition(), 1.0f);

	shaders->SetPerFrameDeferredLightShadowConstantBuffer(
		deviceContext,
		DeferredBufferType::NR_OF_D_ELEMENTS,
		d3d->GetDeferredShaderResourceViews(),
		d3d->GetShadowShaderResourceView());

	FrameWork::Get()->GetDeferredScreenTarget()->Render();*/
}
void SceneManager::RenderDeferredLightSplitPass()
{
}
void SceneManager::RenderDeferredLightMultiplePass()
{
}
void SceneManager::RenderDeferredLightMultipleShadowsPass()
{
}

int SceneManager::GetIndexOfID(int id) const
{
	unsigned int n = m_objects.size();
	for (unsigned int i = 0; i < n; i++)
	{
		if (m_objects[i].id == id)
		{
			return i;
		}
	}
	return -1;
}