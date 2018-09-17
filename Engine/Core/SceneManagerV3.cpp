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

	/*
	Initialize buffers
	*/
	m_viewProjBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(Matrix));
	m_lightBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(LightBuffer));
	if (m_viewProjBufferIndex == -1 || m_lightBufferIndex == -1)
		return false;

	/*
	Initialize matrix buffer
	*/
	m_indexBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(int) * 4);
	m_matrixBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(Matrix) * NR_OF_MATRICES_IN_BUFFER);
	if (m_matrixBufferIndex == -1 || m_indexBufferIndex == -1)
		return false;

	/*
	Initialize light settings
	*/
	LightBuffer lightBuffer;
	lightBuffer.ambientColor = Vector3f(0.05f, 0.05f, 0.05f);
	lightBuffer.nrOfLights = 2U;// MAX_NR_OF_LIGHTS;//min(max(4, 0), MAX_NR_OF_LIGHTS);

	lightBuffer.lights[0].position = Vector3f(-50.0f, 10.0f, -50.0f);
	lightBuffer.lights[0].diffuseColor = Vector3f(1, 1, 1);
	lightBuffer.lights[0].dropoff = -0.01f;
	lightBuffer.lights[1].position = Vector3f(100.0f, 10.0f, 100.0f);
	lightBuffer.lights[1].diffuseColor = Vector3f(1, 1, 1);
	lightBuffer.lights[1].dropoff = -0.01f;


	/*
	Map light data to buffer (is done here because lights don't change (yet) )
	*/
	m_frameWorkManager->GetConstantBufferManager()->MapDataToBuffer(m_lightBufferIndex, &lightBuffer);

	return true;
}

void SceneManagerV3::Render()
{
	ConstantBufferManager* constantBufferManager = m_frameWorkManager->GetConstantBufferManager();
	TextureManager* textureManager = m_frameWorkManager->GetTextureManager();
	VertexBufferManager* vertexBufferManager = m_frameWorkManager->GetVertexBufferManager();

	/*
	Set up geometry pass render targets
	*/
	m_frameWorkManager->SetGeometryPassRenderTargets();

	/*
	Set up sampler state
	*/
	m_frameWorkManager->GetSamplerManager()->SetSamplerStateToPixelShader(SamplerType::CLAMP, 0);

	/*
	Set up geometry pass shaders
	*/
	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::GEOMETRY_PASS);

	/*
	Set view- and projection matrix
	*/
	constantBufferManager->SetConstantBufferToVertexShader(m_viewProjBufferIndex, 0);

	if (m_dataStorage.GetNrOfObjects() > 0)
	{
		unsigned int nrOfFullMatrixBuffers = m_dataStorage.GetNrOfObjects() / NR_OF_MATRICES_IN_BUFFER;
		unsigned int nrOfRemainingObjects = m_dataStorage.GetNrOfObjects() - nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER;

		int* textureIndices = m_dataStorage.GetTextureIndices();
		int* modelIndices = m_dataStorage.GetModelIndices();
		Matrix* worldMatrices = m_dataStorage.GetWorldMatrices();
		//int* objectIndices = m_dataStorage.GetObjectIndices();

		Matrix matrixBuffer[NR_OF_MATRICES_IN_BUFFER];

		TexturedModel* model = nullptr;

		/*
		Render with full matrix buffers
		*/
		for (unsigned int i = 0; i < nrOfFullMatrixBuffers; i++)
		{
			/*
			Copy world matrices into buffer
			*/

			// Changing this loop to memcpy and transposing in Game.cpp has no effect on the FPS
			std::memcpy(matrixBuffer, &worldMatrices[i * NR_OF_MATRICES_IN_BUFFER], sizeof(Matrix) * NR_OF_MATRICES_IN_BUFFER);
			//for (unsigned int j = 0; j < NR_OF_MATRICES_IN_BUFFER; j++)
			//{
			//	unsigned int currentObjectIndex = i * NR_OF_MATRICES_IN_BUFFER + j;
			//	matrixBuffer[j] = worldMatrices[currentObjectIndex];//.GetTranspose();
			//}

			/*
			Map and set the matrix buffer
			*/
			constantBufferManager->MapDataToBuffer(m_matrixBufferIndex, static_cast<void*>(matrixBuffer));
			constantBufferManager->SetConstantBufferToVertexShader(m_matrixBufferIndex, 2);

			for (unsigned int k = 0; k < NR_OF_MATRICES_IN_BUFFER; k++)
			{
				unsigned int currentObjectIndex = i * NR_OF_MATRICES_IN_BUFFER + k;

				/*
				Set texture
				*/
				textureManager->SetTextureToPixelShader(textureIndices[currentObjectIndex], 0);

				/*
				Set vertex buffer
				*/
				model = m_modelManager->GetModel(modelIndices[currentObjectIndex]);
				vertexBufferManager->SetBufferToInputAssembler(model->vertexBufferIndex, 0);

				/*
				Map and set the matrix index buffer
				*/
				int index[] = { static_cast<int>(k), 0, 0, 0 };
				constantBufferManager->MapDataToBuffer(m_indexBufferIndex, static_cast<void*>(index));
				constantBufferManager->SetConstantBufferToVertexShader(m_indexBufferIndex, 1);

				/*
				Render object
				*/
				m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
			}
		}

		/*
		Render the remaining object which did not fit in a full matrix buffer
		*/
		if (nrOfRemainingObjects > 0)
		{
			/*
			Copy world matrices into buffer
			*/

			// Changing this loop to memcpy and transposing in Game.cpp has no effect on the FPS
			std::memcpy(matrixBuffer, &worldMatrices[nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER], sizeof(Matrix) * NR_OF_MATRICES_IN_BUFFER);
			//for (unsigned int i = 0; i < nrOfRemainingObjects; i++)
			//{
			//	unsigned int currentObjectIndex = nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER + i;
			//	matrixBuffer[i] = worldMatrices[currentObjectIndex];// .GetTranspose();
			//}

			/*
			Map and set the matrix buffer
			*/
			constantBufferManager->MapDataToBuffer(m_matrixBufferIndex, matrixBuffer);
			constantBufferManager->SetConstantBufferToVertexShader(m_matrixBufferIndex, 2);

			for (unsigned int k = 0; k < nrOfRemainingObjects; k++)
			{
				unsigned int currentObjectIndex = nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER + k;

				/*
				Set texture
				*/
				textureManager->SetTextureToPixelShader(textureIndices[currentObjectIndex], 0);

				/*
				Set vertex buffer
				*/
				model = m_modelManager->GetModel(modelIndices[currentObjectIndex]);
				vertexBufferManager->SetBufferToInputAssembler(model->vertexBufferIndex, 0);

				/*
				Map and set the matrix index buffer
				*/
				int index[] = { static_cast<int>(k), 0, 0, 0 };
				constantBufferManager->MapDataToBuffer(m_indexBufferIndex, static_cast<void*>(index));
				constantBufferManager->SetConstantBufferToVertexShader(m_indexBufferIndex, 1);

				/*
				Render object
				*/
				m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
			}
		}
	}

	/*
	Set lights
	*/
	constantBufferManager->SetConstantBufferToPixelShader(m_lightBufferIndex, 0);

	/*
	Render light pass
	*/
	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::LIGHT_PASS);
	m_frameWorkManager->SetLightPassRenderTarget();
	m_frameWorkManager->RenderLightPass();

	/*
	Render final pass
	*/
	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::FINAL_PASS);
	m_frameWorkManager->SetFinalPassRenderTarget();
	m_frameWorkManager->RenderFinalPass();
}

void SceneManagerV3::SetViewAndProjectionMatrices(const Matrix & view, const Matrix & projection)
{
	Matrix vp = projection.GetTranspose() * view.GetTranspose();

	m_frameWorkManager->GetConstantBufferManager()->MapDataToBuffer(m_viewProjBufferIndex, static_cast<void*>(&vp));
}

Object* SceneManagerV3::CreateObject()
{
	/*
	Simple "solution" for now
	*/
	return m_dataStorage.CreateObject();

#ifdef SORTABLE_OBJECTS
	for (unsigned int i = 0; i < m_objectTypes.size(); i++)
	{
		if (true)
		{

		}
	}
#endif
}

DataStorage * SceneManagerV3::GetObjectData()
{
	return &m_dataStorage;
}

#ifdef SORTABLE_OBJECTS
void SceneManagerV3::SortObjectInArray(int objectIndex)
{
	
}

void SceneManagerV3::SortObjectArray()
{
	m_objectTypes.clear();

	// Determine the amount of objects of each type
	for (unsigned int i = 0; i < m_nrOfObjects; i++)
	{
		ObjectV3* object = &m_objects[i];
		ObjectType type;
		unsigned int nrOfTypes = m_objectTypes.size();
		bool thisTypeExists = false;

		
		// Determine the type of this object
		type.textureIndex = object->GetTextureIndex();
		type.modelIndex = object->GetModelIndex();


		// Check if this type already exists
		for (unsigned int j = 0; j < nrOfTypes; j++)
		{
			if (type.textureIndex == m_objectTypes[j].textureIndex && type.modelIndex == m_objectTypes[j].modelIndex)
			{
				// Increment the amount of objects with this type
				m_objectTypes[j].nrOfThisType++;
				thisTypeExists = true;
				break;
			}
		}

		// Add this type if it doesn't already exist
		if (!thisTypeExists)
		{
			// At least this object has this type
			type.nrOfThisType = 1;

			type.orderInArray = nrOfTypes;
			m_objectTypes.push_back(type);
		}
	}

	for (unsigned int i = 0; i < m_objectTypes.size(); i++)
	{
		ObjectType currentType = m_objectTypes[i];
		for (unsigned int j = 0; j < m_nrOfObjects; j++)
		{
			if (m_objects[i].GetTextureIndex() == currentType.textureIndex && m_objects[i].GetModelIndex() == currentType.modelIndex)
			{

			}
		}
	}
	int a = 0;
}
#endif