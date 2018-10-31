#include "SceneManagerV3.hpp"
#include "../FrameWork/FrameWorkManager.hpp"
#include "ModelManager.hpp"
#include "MaterialManager.hpp"

#include <thread>

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::steady_clock> Time;

void UpdateObjects(float dt, unsigned int startIndex, unsigned int endIndex, Vector3f* p, Vector3f* m, Math::Matrix* rm, Vector3f* ra, float* rs, Vector3f* s, Math::Matrix* wm)
{
	for (unsigned int i = startIndex; i < endIndex; i++)
	{
		p[i] += m[i] * dt;

		// Global rotation
		rm[i] *= Math::Matrix::Rotation(ra[i], rs[i] * dt);

		// Local rotation
		//rotationMatrices[i] = rotationMatrices[i] * Matrix::Rotation(rotationAxises[i], rotationSpeeds[i] * dt);
		wm[i] = Math::Matrix::World(p[i], rm[i], s[i]);
	}
}


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
	m_viewProjBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(Math::Matrix));
	m_lightBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(LightBuffer));
	if (m_viewProjBufferIndex == -1 || m_lightBufferIndex == -1)
		return false;

	/*
	Initialize matrix buffer
	*/
	m_idBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(int) * 4);
	m_matrixBufferIndex = m_frameWorkManager->GetConstantBufferManager()->CreateConstantBuffer(sizeof(Math::Matrix) * NR_OF_MATRICES_IN_BUFFER);
	if (m_matrixBufferIndex == -1 || m_idBufferIndex == -1)
		return false;

	return true;
}

void SceneManagerV3::Update(float dt)
{
	/*
	Nr of cubes	| FPS normal	| FPS using 3 additional threads
	------------|---------------|-------------------------------
	8128		| ~1050			| ~400
	65280		| ~166			| ~190

	*/


	ObjectData* objectData = m_dataStorage.GetObjectData();

	unsigned int nThreads = 4U;
	unsigned int nObjsPerThread = m_dataStorage.GetNrOfObjects() / nThreads;

	
	// Launch 3 additional threads
	std::thread threads[3];
	for (unsigned int i = 0; i < nThreads - 1; i++)
	{
		threads[i] = std::thread(
			UpdateObjects,
			dt,
			nObjsPerThread * i,
			nObjsPerThread * (i + 1),
			objectData->positions,
			objectData->movements,
			objectData->rotationMatrices,
			objectData->rotationAxises,
			objectData->rotationSpeeds,
			objectData->scales,
			objectData->worldMatrices
		);
	}
	
	
	// Run main thread
	UpdateObjects(
		dt,
		//0U,
		nObjsPerThread * (nThreads - 1),
		m_dataStorage.GetNrOfObjects(),
		objectData->positions,
		objectData->movements,
		objectData->rotationMatrices,
		objectData->rotationAxises,
		objectData->rotationSpeeds,
		objectData->scales,
		objectData->worldMatrices
	);

	for (unsigned int i = 0; i < nThreads - 1; i++)
	{
		threads[i].join();
	}


	/*unsigned int nrOfObjects = m_dataStorage.GetNrOfObjects();

	ObjectData* objectData = m_dataStorage.GetObjectData();
	
	Vector3f* positions = objectData->positions;
	Vector3f* movements = objectData->movements;
	
	Math::Matrix* rotationMatrices = objectData->rotationMatrices;
	Vector3f* rotationAxises = objectData->rotationAxises;
	float* rotationSpeeds = objectData->rotationSpeeds;

	Vector3f* scales = objectData->scales;

	Math::Matrix* worldMatrices = objectData->worldMatrices;

	

	for (unsigned int i = 0; i < nrOfObjects; i++)
	{
		positions[i] += movements[i] * dt;

		// Global rotation
		rotationMatrices[i] *= Math::Matrix::Rotation(rotationAxises[i], rotationSpeeds[i] * dt);

		// Local rotation
		//rotationMatrices[i] = rotationMatrices[i] * Matrix::Rotation(rotationAxises[i], rotationSpeeds[i] * dt);
		worldMatrices[i] = Math::Matrix::World(positions[i], rotationMatrices[i], scales[i]);
	}*/
}

void SceneManagerV3::Render()
{
	//RenderGeometryPass();
	RenderInstancedGeometryPass();
	RenderLightPass();
	RenderFinalPass();
}

void SceneManagerV3::RenderGeometryPass()
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

	unsigned int nrOfObjects = m_dataStorage.GetNrOfObjects();
	if (nrOfObjects > 0)
	{
		unsigned int nrOfFullMatrixBuffers = nrOfObjects / NR_OF_MATRICES_IN_BUFFER;
		unsigned int nrOfRemainingObjects = nrOfObjects - nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER;

		ObjectData* objectData = m_dataStorage.GetObjectData();
		int* textureIndices = objectData->textureIndices;
		int* modelIndices = objectData->modelIndices;
		Math::Matrix* worldMatrices = objectData->worldMatrices;

		Math::Matrix matrixBuffer[NR_OF_MATRICES_IN_BUFFER];

		TexturedModel* model = nullptr;

		/*
		Render with full matrix buffers
		*/
		for (unsigned int i = 0; i < nrOfFullMatrixBuffers; i++)
		{
			/*
			Copy world matrices into buffer
			*/

			// Changing this loop to memcpy and transposing in Game.cpp had no effect on the FPS
			//std::memcpy(matrixBuffer, &worldMatrices[i * NR_OF_MATRICES_IN_BUFFER], sizeof(Matrix) * NR_OF_MATRICES_IN_BUFFER);
			for (unsigned int j = 0; j < NR_OF_MATRICES_IN_BUFFER; j++)
			{
				unsigned int currentObjectIndex = i * NR_OF_MATRICES_IN_BUFFER + j;
				matrixBuffer[j] = worldMatrices[currentObjectIndex].GetTranspose();
			}

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
				constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
				constantBufferManager->SetConstantBufferToVertexShader(m_idBufferIndex, 1);

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

			// Changing this loop to memcpy and transposing in Game.cpp had no effect on the FPS
			//std::memcpy(matrixBuffer, &worldMatrices[nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER], sizeof(Matrix) * NR_OF_MATRICES_IN_BUFFER);
			for (unsigned int i = 0; i < nrOfRemainingObjects; i++)
			{
				unsigned int currentObjectIndex = nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER + i;
				matrixBuffer[i] = worldMatrices[currentObjectIndex].GetTranspose();
			}

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
				constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
				constantBufferManager->SetConstantBufferToVertexShader(m_idBufferIndex, 1);

				/*
				Render object
				*/
				m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
			}
		}
	}
}
void SceneManagerV3::RenderInstancedGeometryPass()
{
	ConstantBufferManager* constantBufferManager = m_frameWorkManager->GetConstantBufferManager();
	TextureManager* textureManager = m_frameWorkManager->GetTextureManager();
	VertexBufferManager* vertexBufferManager = m_frameWorkManager->GetVertexBufferManager();

	/*
	Set up instanced geometry pass render targets (same as regular geometry pass)
	*/
	m_frameWorkManager->SetGeometryPassRenderTargets();

	/*
	Set up sampler state (same as regular geometry pass)
	*/
	m_frameWorkManager->GetSamplerManager()->SetSamplerStateToPixelShader(SamplerType::CLAMP, 0);

	/*
	Set up instanced geometry pass shaders
	*/
	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::INSTANCED_GEOMETRY_PASS);

	/*
	Set view- and projection matrix (same as regular geometry pass)
	*/
	constantBufferManager->SetConstantBufferToGeometryShader(m_viewProjBufferIndex, 0);


#ifdef ONLY_28_MATRICES
	int nrOfObjects = m_dataStorage.GetNrOfObjects();
	if (nrOfObjects > 0)
	{
		int nrOfFullMatrixBuffers = nrOfObjects / NR_OF_MATRICES_IN_BUFFER;
		int nrOfRemainingObjects = nrOfObjects - nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER;
		const int maxNrOfInstances = 28U;

		ObjectData* objectData = m_dataStorage.GetObjectData();
		int* textureIndices = objectData->textureIndices;
		int* modelIndices = objectData->modelIndices;
		Math::Matrix* worldMatrices = objectData->worldMatrices;

		Math::Matrix matrixBuffer[NR_OF_MATRICES_IN_BUFFER];

		TexturedModel* model = nullptr;

		/*
		Render with full matrix buffers
		*/
		for (int i = 0; i < nrOfFullMatrixBuffers; i++)
		{
			/*
			Copy world matrices into buffer
			*/
			for (int j = 0; j < NR_OF_MATRICES_IN_BUFFER; j++)
			{
				matrixBuffer[j] = worldMatrices[i * NR_OF_MATRICES_IN_BUFFER + j].GetTranspose();
			}

			/*
			Map and set the matrix buffer
			*/
			constantBufferManager->MapDataToBuffer(m_matrixBufferIndex, static_cast<void*>(matrixBuffer));
			constantBufferManager->SetConstantBufferToGeometryShader(m_matrixBufferIndex, 2);

			/*
			Set texture
			*/
			textureManager->SetTextureToPixelShader(textureIndices[0], 0);

			/*
			Set the vertex buffer
			*/
			model = m_modelManager->GetModel(modelIndices[0]);
			vertexBufferManager->SetBufferToInputAssembler(model->vertexBufferIndex, 0);

			/*
			Map and set the instance index buffer
			*/
			int index[] = { 0, maxNrOfInstances, 0, 0 };
			constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
			constantBufferManager->SetConstantBufferToGeometryShader(m_idBufferIndex, 1);

			/*
			Render object
			*/
			m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
		}

		/*
		Render the remaining object which did not fit in a full matrix buffer
		*/
		if (nrOfRemainingObjects > 0)
		{
			/*
			Copy world matrices into buffer
			*/
			for (int i = 0; i < nrOfRemainingObjects; i++)
			{
				matrixBuffer[i] = worldMatrices[nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER + i].GetTranspose();
			}

			/*
			Map and set the matrix buffer
			*/
			constantBufferManager->MapDataToBuffer(m_matrixBufferIndex, matrixBuffer);
			constantBufferManager->SetConstantBufferToGeometryShader(m_matrixBufferIndex, 2);

			/*
			Set texture
			*/
			textureManager->SetTextureToPixelShader(textureIndices[0], 0);

			/*
			Set the vertex buffer
			*/
			model = m_modelManager->GetModel(modelIndices[0]);
			vertexBufferManager->SetBufferToInputAssembler(model->vertexBufferIndex, 0);

			/*
			Set the instance index buffer
			*/
			int index[] = { 0, nrOfRemainingObjects, 0, 0 };
			constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
			constantBufferManager->SetConstantBufferToGeometryShader(m_idBufferIndex, 1);

			/*
			Render object
			*/
			m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
		}
	}


#else
	unsigned int nrOfObjects = m_dataStorage.GetNrOfObjects();
	if (nrOfObjects > 0)
	{
		unsigned int nrOfFullMatrixBuffers = nrOfObjects / NR_OF_MATRICES_IN_BUFFER;
		unsigned int nrOfRemainingObjects = nrOfObjects - nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER;
		const unsigned int maxNrOfInstances = 28U;

		ObjectData* objectData = m_dataStorage.GetObjectData();
		int* textureIndices = objectData->textureIndices;
		int* modelIndices = objectData->modelIndices;
		Math::Matrix* worldMatrices = objectData->worldMatrices;

		Math::Matrix matrixBuffer[NR_OF_MATRICES_IN_BUFFER];

		TexturedModel* model = nullptr;

		/*
		========================================================================================================
		*/
		/*
		Render with full matrix buffers
		*/
		for (unsigned int i = 0; i < nrOfFullMatrixBuffers; i++)
		{
			/*
			Copy world matrices into buffer
			*/

			// Changing this loop to memcpy and transposing in Game.cpp had no effect on the FPS
			//std::memcpy(matrixBuffer, &worldMatrices[i * NR_OF_MATRICES_IN_BUFFER], sizeof(Matrix) * NR_OF_MATRICES_IN_BUFFER);
			for (unsigned int j = 0; j < NR_OF_MATRICES_IN_BUFFER; j++)
			{
				unsigned int currentObjectIndex = i * NR_OF_MATRICES_IN_BUFFER + j;
				matrixBuffer[j] = worldMatrices[currentObjectIndex].GetTranspose();
			}

			/*
			Map and set the matrix buffer
			*/
			constantBufferManager->MapDataToBuffer(m_matrixBufferIndex, static_cast<void*>(matrixBuffer));
			constantBufferManager->SetConstantBufferToGeometryShader(m_matrixBufferIndex, 2);

			/*
			Set texture
			*/
			textureManager->SetTextureToPixelShader(textureIndices[0], 0);

			/*
			Set the vertex buffer
			*/
			model = m_modelManager->GetModel(modelIndices[0]);
			vertexBufferManager->SetBufferToInputAssembler(model->vertexBufferIndex, 0);

			/*
			========================================================================================================
			*/

			/*
			Map and set the matrix index buffer
			*/
			unsigned int nrOfFullInstancedCalls = NR_OF_MATRICES_IN_BUFFER / maxNrOfInstances;
			unsigned int nrOfInstancedObjectsFullCalls = nrOfFullInstancedCalls * maxNrOfInstances;
			unsigned int nrOfRemainingInstancedObjects = NR_OF_MATRICES_IN_BUFFER - nrOfInstancedObjectsFullCalls;

			/*
			========================================================================================================
			*/
			/*
			Full instance passes
			*/
			for (unsigned int i = 0; i < nrOfFullInstancedCalls; i++)
			{
				int index[] = { static_cast<int>(i * maxNrOfInstances), static_cast<int>(maxNrOfInstances), 0, 0 };
				constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
				constantBufferManager->SetConstantBufferToGeometryShader(m_idBufferIndex, 1);

				/*
				Render object
				*/
				m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
			}

			/*
			========================================================================================================
			*/
			/*
			Remaining instance pass
			*/
			int index[] = { static_cast<int>(nrOfInstancedObjectsFullCalls), static_cast<int>(nrOfRemainingInstancedObjects), 0, 0 };
			constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
			constantBufferManager->SetConstantBufferToGeometryShader(m_idBufferIndex, 1);

			/*
			Render object
			*/
			m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
		}

		/*
		========================================================================================================
		*/
		/*
		Render the remaining object which did not fit in a full matrix buffer
		*/
		if (nrOfRemainingObjects > 0)
		{
			/*
			Copy world matrices into buffer
			*/

			// Changing this loop to memcpy and transposing in Game.cpp had no effect on the FPS
			//std::memcpy(matrixBuffer, &worldMatrices[nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER], sizeof(Matrix) * NR_OF_MATRICES_IN_BUFFER);
			for (unsigned int i = 0; i < nrOfRemainingObjects; i++)
			{
				unsigned int currentObjectIndex = nrOfFullMatrixBuffers * NR_OF_MATRICES_IN_BUFFER + i;
				matrixBuffer[i] = worldMatrices[currentObjectIndex].GetTranspose();
			}

			/*
			Map and set the matrix buffer
			*/
			constantBufferManager->MapDataToBuffer(m_matrixBufferIndex, matrixBuffer);
			constantBufferManager->SetConstantBufferToGeometryShader(m_matrixBufferIndex, 2);

			/*
			Set texture
			*/
			textureManager->SetTextureToPixelShader(textureIndices[0], 0);

			/*
			Set the vertex buffer
			*/
			model = m_modelManager->GetModel(modelIndices[0]);
			vertexBufferManager->SetBufferToInputAssembler(model->vertexBufferIndex, 0);

			/*
			========================================================================================================
			*/

			/*
			Map and set the matrix index buffer
			*/
			unsigned int nrOfFullInstancedCalls = nrOfRemainingObjects / maxNrOfInstances;
			unsigned int nrOfInstancedObjectsFullCalls = nrOfFullInstancedCalls * maxNrOfInstances;
			unsigned int nrOfRemainingInstancedObjects = nrOfRemainingObjects - nrOfInstancedObjectsFullCalls;

			/*
			========================================================================================================
			*/
			/*
			Full instance passes
			*/
			for (unsigned int i = 0; i < nrOfFullInstancedCalls; i++)
			{
				int index[] = { static_cast<int>(i * maxNrOfInstances), static_cast<int>(maxNrOfInstances), 0, 0 };
				constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
				constantBufferManager->SetConstantBufferToGeometryShader(m_idBufferIndex, 1);

				/*
				Render object
				*/
				m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
			}

			/*
			========================================================================================================
			*/
			/*
			Remaining instance pass
			*/
			int index[] = { static_cast<int>(nrOfInstancedObjectsFullCalls), static_cast<int>(nrOfRemainingInstancedObjects), 0, 0 };
			constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
			constantBufferManager->SetConstantBufferToGeometryShader(m_idBufferIndex, 1);

			/*
			Render object
			*/
			m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
		}
	}
#endif













#ifdef REMOVED_CODE
	unsigned int nrOfObjects = NR_OF_MATRICES_IN_BUFFER;// m_dataStorage.GetNrOfObjects();
	//unsigned int nrOfObjects = m_dataStorage.GetNrOfObjects();
	if (nrOfObjects > 0)
	{
		ObjectData* objectData = m_dataStorage.GetObjectData();
		int* textureIndices = objectData->textureIndices;
		int* modelIndices = objectData->modelIndices;
		Math::Matrix* worldMatrices = objectData->worldMatrices;

		Math::Matrix matrixBuffer[NR_OF_MATRICES_IN_BUFFER];

		TexturedModel* model = nullptr;


		for (unsigned int i = 0; i < nrOfObjects; i++)
		{
			matrixBuffer[i] = worldMatrices[i].GetTranspose();
		}

		/*
		Map and set the matrix buffer
		*/
		constantBufferManager->MapDataToBuffer(m_matrixBufferIndex, matrixBuffer);
		constantBufferManager->SetConstantBufferToGeometryShader(m_matrixBufferIndex, 2);

		/*
		Set texture
		*/
		textureManager->SetTextureToPixelShader(textureIndices[0], 0);

		/*
		Set the vertex buffer
		*/
		model = m_modelManager->GetModel(modelIndices[0]);
		vertexBufferManager->SetBufferToInputAssembler(model->vertexBufferIndex, 0);

		const unsigned int maxNrOfInstances = 28U;
		/*
		Map and set the matrix index buffer
		*/
		unsigned int nrOfFullInstancedCalls = nrOfObjects / maxNrOfInstances;
		unsigned int nrOfObjectsRenderedInFullCalls = nrOfFullInstancedCalls * maxNrOfInstances;
		unsigned int nrOfRemainingObjects = nrOfObjects - nrOfObjectsRenderedInFullCalls;

		/*
		Full instance passes
		*/
		for (unsigned int i = 0; i < nrOfFullInstancedCalls; i++)
		{
			int index[] = { i * maxNrOfInstances, maxNrOfInstances, 0, 0 };
			constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
			constantBufferManager->SetConstantBufferToGeometryShader(m_idBufferIndex, 1);
			
			/*
			Render object
			*/
			m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
		}

		/*
		Remaining instance pass
		*/
		int index[] = { nrOfObjectsRenderedInFullCalls, maxNrOfInstances, 0, 0 };
		constantBufferManager->MapDataToBuffer(m_idBufferIndex, static_cast<void*>(index));
		constantBufferManager->SetConstantBufferToGeometryShader(m_idBufferIndex, 1);

		/*
		Render object
		*/
		m_frameWorkManager->RenderGeometryPassWithCurrentSettings(model->nrOfVertices);
	}
#endif
}
void SceneManagerV3::RenderLightPass()
{
	ConstantBufferManager* constantBufferManager = m_frameWorkManager->GetConstantBufferManager();

	/*
	Set shaders
	*/
	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::LIGHT_PASS);

	/*
	Set render targets
	*/
	m_frameWorkManager->SetLightPassRenderTarget();

	/*
	Copy light data
	*/
	LightData* lightData = m_dataStorage.GetLightData();
	LightBuffer lightBuffer;
	lightBuffer.nrOfLights = m_dataStorage.GetNrOfLights();
	lightBuffer.ambientColor = lightData->ambientColor;

	for (int i = 0; i < lightBuffer.nrOfLights; i++)
	{
		lightBuffer.lights[i].position = lightData->positions[i];
		lightBuffer.lights[i].diffuseColor = lightData->diffuseColors[i];
		lightBuffer.lights[i].dropoff = lightData->dropoffs[i];
	}

	/*
	Map light data
	*/
	m_frameWorkManager->GetConstantBufferManager()->MapDataToBuffer(m_lightBufferIndex, &lightBuffer);

	/*
	Set light data to shader
	*/
	constantBufferManager->SetConstantBufferToPixelShader(m_lightBufferIndex, 0);

	/*
	Render light pass
	*/
	m_frameWorkManager->RenderLightPass();
}
void SceneManagerV3::RenderFinalPass()
{
	m_frameWorkManager->GetShaderManager()->SetShaders(ShaderTypeV2::FINAL_PASS);
	m_frameWorkManager->SetFinalPassRenderTarget();
	m_frameWorkManager->RenderFinalPass();
}

#ifdef SORT_OBJECTS_BY_TEXTURE
void SceneManagerV3::SortObjectsInDataStorage()
{
	ObjectData* objectData = m_dataStorage.GetObjectData();
	
	int textureIndex = -1;

	for (unsigned int i = 0; i < m_dataStorage.GetNrOfObjects(); i++)
	{
		if (objectData->textureIndices[i] > textureIndex)
		{
			objectData->
		}
		else
		{

		}
	}
}
#endif

void SceneManagerV3::SetViewAndProjectionMatrices(const Math::Matrix & view, const Math::Matrix & projection)
{
	Math::Matrix vp = projection.GetTranspose() * view.GetTranspose();

	m_frameWorkManager->GetConstantBufferManager()->MapDataToBuffer(m_viewProjBufferIndex, static_cast<void*>(&vp));
}

ObjectV4* SceneManagerV3::CreateObject()
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

DataStorage * SceneManagerV3::GetData()
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