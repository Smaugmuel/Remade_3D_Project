#include "Scene.hpp"

#include "TextureObject.hpp"
#include "SingleColorObject.hpp"

#include "RenderManager.hpp"

#include <fstream>
#include <sstream>

Scene::Scene()
{
	m_nrOfTexturedObjects = 0;
	m_nrOfSingleColoredObjects = 0;

	m_sceneName = "";
}
Scene::~Scene()
{
	Reset();
}

void Scene::Update(float dt)
{
	for (unsigned int i = 0; i < m_nrOfTexturedObjects; i++)
	{
		//m_texturedObjects[i]->Rotate(0, dt, 0);
		//m_texturedObjects[i].Update();
		m_texturedObjects[i]->Update();
	}

	for (unsigned int i = 0; i < m_nrOfSingleColoredObjects; i++)
	{
		//m_singleColoredObjects[i].Rotate(0, dt, 0);
		//m_singleColoredObjects[i].Update();
		m_singleColoredObjects[i]->Update();
	}
}

void Scene::LoadIntoRenderManager()
{
	RenderManager* renderManager = RenderManager::Get();
	renderManager->Reset();

	for (unsigned int i = 0; i < m_nrOfTexturedObjects; i++)
	{
		renderManager->AddTexturedObject(m_texturedObjects[i]);
	}

	for (unsigned int i = 0; i < m_nrOfSingleColoredObjects; i++)
	{
		renderManager->AddSingleColoredObject(m_singleColoredObjects[i]);
	}
}

void Scene::AddTexturedObject(const std::string & modelName, const std::string & textureName, const Vector3f & position, const Vector3f & rotation, const Vector3f & scale)
{
	if (m_nrOfTexturedObjects >= MAX_NR_OF_TEXTURED_OBJECTS)
	{
		return;
	}

	TextureObject* object = new TextureObject;
	if (!object->Initialize(modelName, textureName))
	{
		delete object;
		return;
	}

	object->SetPosition(position);
	object->SetRotation(rotation);
	object->SetScale(scale);

	object->Update();

	//m_texturedObjects.push_back(object);
	m_texturedObjects[m_nrOfTexturedObjects++] = object;
}
void Scene::AddSingleColoredObject(const std::string & modelName, const Vector3f & color, const Vector3f & position, const Vector3f & rotation, const Vector3f & scale)
{
	if (m_nrOfSingleColoredObjects >= MAX_NR_OF_SINGLE_COLORED_OBJECTS)
	{
		return;
	}

	SingleColorObject* object = new SingleColorObject;
	if (!object->Initialize(modelName, color))
	{
		delete object;
		return;
	}

	object->SetPosition(position);
	object->SetRotation(rotation);
	object->SetScale(scale);

	object->Update();

	//m_singleColoredObjects.push_back(object);
	m_singleColoredObjects[m_nrOfSingleColoredObjects++] = object;
}

bool Scene::LoadFromFile(std::string fileName)
{
	std::ifstream inFile("../Scenes/" + fileName + ".txt");
	std::string line, temp;
	
	unsigned int nrOfTexturedObjects = 0;
	unsigned int nrOfSingleColoredObjects = 0;

	TexturedObjectData tData;
	SingleColoredObjectData cData;

	if (!inFile.is_open())
	{
		return false;
	}


	Reset();


	/* ===================================== Read scene name ====================================== */
	std::getline(inFile, line);
	std::istringstream issSceneName(line);

	temp = "";
	issSceneName >> temp;		// Remove prefix
	issSceneName >> m_sceneName;

	/* ===================================== Read empty line ====================================== */
	std::getline(inFile, line);

	/* ============================= Read number of textured objects ============================== */
	std::getline(inFile, line);
	std::istringstream issNT(line);

	temp = "";
	issNT >> temp;		// Remove prefix
	issNT >> nrOfTexturedObjects;

	/* ============================= Read number of colored objects =============================== */
	std::getline(inFile, line);
	std::istringstream issNC(line);

	temp = "";
	issNC >> temp;		// Remove prefix
	issNC >> nrOfSingleColoredObjects;


	/* ================================== Read textured objects =================================== */
	for (unsigned int i = 0; i < nrOfTexturedObjects; i++)
	{
		/* --------------------------------- Read empty line -------------------------------------- */
		std::getline(inFile, line);

		/* ---------------------------------- Read position --------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issP(line);
		temp = "";

		// Remove prefix
		issP >> temp >> tData.p.x >> tData.p.y >> tData.p.z;

		/* ---------------------------------- Read rotation --------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issR(line);

		// Remove prefix
		issR >> temp >> tData.r.x >> tData.r.y >> tData.r.z;

		/* ------------------------------------ Read scale ---------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issS(line);

		// Remove prefix
		issS >> temp >> tData.s.x >> tData.s.y >> tData.s.z;

		/* --------------------------------- Read model name -------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issM(line);

		// Remove prefix
		issM >> temp >> tData.m;

		/* -------------------------------- Read texture name ------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issT(line);

		// Remove prefix
		issT >> temp >> tData.t;

		/* -------------------------------- Create the object ------------------------------------- */
		AddTexturedObject(tData.m, tData.t, tData.p, tData.r, tData.s);
	}

	/* ===================================== Read empty line ===================================== */
	std::getline(inFile, line);

	/* ================================== Read colored objects ==================================== */
	for (unsigned int i = 0; i < nrOfSingleColoredObjects; i++)
	{
		/* --------------------------------- Read empty line -------------------------------------- */
		std::getline(inFile, line);

		/* ---------------------------------- Read position --------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issP(line);
		temp = "";

		// Remove prefix
		issP >> temp >> cData.p.x >> cData.p.y >> cData.p.z;

		/* ---------------------------------- Read rotation --------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issR(line);

		// Remove prefix
		issR >> temp >> cData.r.x >> cData.r.y >> cData.r.z;

		/* ------------------------------------ Read scale ---------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issS(line);

		// Remove prefix
		issS >> temp >> cData.s.x >> cData.s.y >> cData.s.z;

		/* --------------------------------- Read model name -------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issM(line);

		// Remove prefix
		issM >> temp >> cData.m;

		/* ------------------------------------ Read color ---------------------------------------- */
		std::getline(inFile, line);
		std::istringstream issC(line);

		// Remove prefix
		issC >> temp >> cData.c.x >> cData.c.y >> cData.c.z;

		/* -------------------------------- Create the object ------------------------------------- */
		AddSingleColoredObject(cData.m, cData.c, cData.p, cData.r, cData.s);
	}

	return true;
}
void Scene::SaveToFile(std::string fileName)
{
	std::ofstream outFile("../Scenes/" + fileName + ".txt");
	TexturedObjectData tData;
	SingleColoredObjectData cData;

	/* Format (without the <>):

	Scene <Name of scene file>
	
	nT <Number of textured objects>
	nC <Number of colored objects>
	
	p <X position> <Y position> <Z position>				// Here begins the textured objects
	r <X rotation> <Y rotation> <Z rotation>
	s <X scale> <Y scale> <Z scale>
	m <Model name>
	t <Texture name>

	p <X position> <Y position> <Z position>
	r <X rotation> <Y rotation> <Z rotation>
	s <X scale> <Y scale> <Z scale>
	m <Model name>
	t <Texture name>

	p <X position> <Y position> <Z position>
	r <X rotation> <Y rotation> <Z rotation>
	s <X scale> <Y scale> <Z scale>
	m <Model name>
	t <Texture name>

	p <X position> <Y position> <Z position>
	r <X rotation> <Y rotation> <Z rotation>
	s <X scale> <Y scale> <Z scale>
	m <Model name>
	t <Texture name>


	p <X position> <Y position> <Z position>				// Here begins the colored objects
	r <X rotation> <Y rotation> <Z rotation>
	s <X scale> <Y scale> <Z scale>
	m <Model name>
	c <R color> <G color> <B color>

	p <X position> <Y position> <Z position>
	r <X rotation> <Y rotation> <Z rotation>
	s <X scale> <Y scale> <Z scale>
	m <Model name>
	c <R color> <G color> <B color>

	*/

	outFile << "Scene " << fileName << "\n\n";
	outFile << "nT " << m_nrOfTexturedObjects << "\n";
	outFile << "nC " << m_nrOfSingleColoredObjects << "\n\n";

	for (unsigned int i = 0; i < m_nrOfTexturedObjects; i++)
	{
		tData.p = m_texturedObjects[i]->GetPosition();
		tData.r = m_texturedObjects[i]->GetRotation();
		tData.s = m_texturedObjects[i]->GetScale();
		tData.m = m_texturedObjects[i]->GetModelName();
		tData.t = m_texturedObjects[i]->GetTextureName();
		
		outFile << "p " << tData.p.x << " " << tData.p.y << " " << tData.p.z << "\n";
		outFile << "r " << tData.r.x << " " << tData.r.y << " " << tData.r.z << "\n";
		outFile << "s " << tData.s.x << " " << tData.s.y << " " << tData.s.z << "\n";
		outFile << "m " << tData.m << "\n";
		outFile << "t " << tData.t << "\n\n";
	}

	outFile << "\n";

	for (unsigned int i = 0; i < m_nrOfSingleColoredObjects; i++)
	{
		cData.p = m_singleColoredObjects[i]->GetPosition();
		cData.r = m_singleColoredObjects[i]->GetRotation();
		cData.s = m_singleColoredObjects[i]->GetScale();
		cData.m = m_singleColoredObjects[i]->GetModelName();
		cData.c = m_singleColoredObjects[i]->GetColor();

		outFile << "p " << cData.p.x << " " << cData.p.y << " " << cData.p.z << "\n";
		outFile << "r " << cData.r.x << " " << cData.r.y << " " << cData.r.z << "\n";
		outFile << "s " << cData.s.x << " " << cData.s.y << " " << cData.s.z << "\n";
		outFile << "m " << cData.m << "\n";
		outFile << "c " << cData.c.x << " " << cData.c.y << " " << cData.c.z << "\n\n";
	}
}

void Scene::Reset()
{
	/*for (unsigned int i = 0; i < m_texturedObjects.size(); i++)
	{
		delete m_texturedObjects[i];
		m_texturedObjects[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_singleColoredObjects.size(); i++)
	{
		delete m_singleColoredObjects[i];
		m_singleColoredObjects[i] = nullptr;
	}*/
	for (unsigned int i = 0; i < m_nrOfTexturedObjects; i++)
	{
		delete m_texturedObjects[i];
		m_texturedObjects[i] = nullptr;
	}
	for (unsigned int i = 0; i < m_nrOfSingleColoredObjects; i++)
	{
		delete m_singleColoredObjects[i];
		m_singleColoredObjects[i] = nullptr;
	}

	m_nrOfTexturedObjects = 0;
	m_nrOfSingleColoredObjects = 0;

	m_sceneName = "";
}

TextureObject ** Scene::GetTexturedObjects()
{
	return m_texturedObjects;
}
SingleColorObject ** Scene::GetSingleColoredObjects()
{
	return m_singleColoredObjects;
}

unsigned int Scene::GetNrOfTexturedObjects() const
{
	return m_nrOfTexturedObjects;
}
unsigned int Scene::GetNrOfSingleColoredObjects() const
{
	return m_nrOfSingleColoredObjects;
}

const std::string & Scene::GetName() const
{
	return m_sceneName;
}

//const std::vector<TextureObject*>& Scene::GetTexturedObjects() const
//{
//	return m_texturedObjects;
//}
//
//const std::vector<SingleColorObject*>& Scene::GetSingleColoredObjects() const
//{
//	return m_singleColoredObjects;
//}

//void Scene::Render()
//{
//	if (!m_texturedObjects.empty())
//	{
//		RenderTexturedObjects();
//	}
//	if (!m_singleColoredObjects.empty())
//	{
//		RenderSingleColoredObjects();
//	}
//}

//void Scene::RenderTexturedObjects()
//{
//	Direct3D* d3d = Direct3D::Get();
//	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
//	ShaderManager* shaders = ShaderManager::Get();
//	Camera* cam = PlayerCameraManager::Get()->GetCurrentCamera();
//
//	ModelStorage* modelStorage = ModelStorage::Get();
//	TextureStorage* textureStorage = TextureStorage::Get();
//	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();
//	TextureModel* textureModel;
//	SingleColorModel* singleColorModel;
//	ID3D11ShaderResourceView* texture;
//
//	DirectX::XMMATRIX worldMatrices[CHUNK_SIZE];
//
//	unsigned int nrOfFullRenderPasses;
//	unsigned int nrOfCubesInFullRenderPasses;
//	unsigned int nrOfRemainingCubes;
//
//
//
//	d3d->SetDeferredTargets();
//	d3d->ClearDeferredTargets();
//
//	bufferStorage->SetGSViewMatrix(deviceContext, cam->GetViewMatrix());
//	bufferStorage->SetGSProjectionMatrix(deviceContext, cam->GetProjectionMatrix());
//
//	/* ========================= Render texture objects ========================== */
//	shaders->SetShaderType(deviceContext, ShaderType::D_TEXTURE_CHUNK);
//
//	/* ------------------------- Render cubes ------------------------- */
//	textureModel = modelStorage->GetTextureModel(m_texturedObjects[0]->GetModelName());
//	textureModel->SetupRender(deviceContext);
//
//	texture = textureStorage->GetTexture(m_texturedObjects[0]->GetTextureName());
//	shaders->SetPerObjectDeferredTextureChunkShaderGroup(deviceContext, texture);
//
//	nrOfFullRenderPasses = m_texturedObjects.size() / CHUNK_SIZE;
//	nrOfCubesInFullRenderPasses = nrOfFullRenderPasses * CHUNK_SIZE;
//	nrOfRemainingCubes = m_texturedObjects.size() - nrOfCubesInFullRenderPasses;
//
//	// Render the full chunks of objects
//	for (unsigned int i = 0; i < nrOfFullRenderPasses; i++)
//	{
//		for (unsigned int j = 0; j < CHUNK_SIZE; j++)
//		{
//			DirectX::XMMATRIX temp = m_texturedObjects[i * CHUNK_SIZE + j]->GetWorldMatrix();
//			memcpy(&worldMatrices[j], &temp, sizeof(temp));
//		}
//
//		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
//		bufferStorage->SetGSNrOfObjects(deviceContext, CHUNK_SIZE);
//		textureModel->Render(deviceContext);
//	}
//
//	// Render the leftovers
//	if (nrOfRemainingCubes > 0)
//	{
//		for (unsigned int i = 0; i < nrOfRemainingCubes; i++)
//		{
//			DirectX::XMMATRIX temp = m_texturedObjects[nrOfCubesInFullRenderPasses + i]->GetWorldMatrix();
//			memcpy(&worldMatrices[i], &temp, sizeof(temp));
//		}
//
//		bufferStorage->SetGSWorldMatrixArray(deviceContext, worldMatrices);
//		bufferStorage->SetGSNrOfObjects(deviceContext, nrOfRemainingCubes);
//		textureModel->Render(deviceContext);
//	}
//}
//
//void Scene::RenderSingleColoredObjects()
//{
//}