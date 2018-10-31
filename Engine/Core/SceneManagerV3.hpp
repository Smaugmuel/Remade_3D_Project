#ifndef SCENE_MANAGER_V3_HPP
#define SCENE_MANAGER_V3_HPP
#include "DataStorage.hpp"
#include <vector>

//#define ONLY_28_MATRICES
//#define NR_OF_MATRICES_IN_BUFFER 28

// The constant buffer size limit is 4096 float4's, which means 1024 float4x4's

#define NR_OF_MATRICES_IN_BUFFER 1024


static const unsigned int MAX_NR_OF_OBJECTS = MAX_NR_OF_OBJECT_INSTANCES;

//#define SORTABLE_OBJECTS
//#define SORT_OBJECTS_BY_TEXTURE

class ModelManager;
class MaterialManager;
class FrameWorkManager;

struct LightBuffer
{
	struct CBufferLight
	{
		Vector3f position;
		float dropoff = -0.01f;
		Vector3f diffuseColor;
		float padding = 0.0f;
	};

	CBufferLight lights[MAX_NR_OF_LIGHT_INSTANCES];
	Vector3f ambientColor;
	int nrOfLights;
};

class SceneManagerV3 final
{
public:
	SceneManagerV3();
	~SceneManagerV3();

	bool Initialize(ModelManager* modelManager, MaterialManager* materialManager, FrameWorkManager* frameWorkManager);

	void Update(float dt);

	void Render();

	void SetViewAndProjectionMatrices(const Math::Matrix& view, const Math::Matrix& projection);

	ObjectV4* CreateObject();
	//int CreateObject();
	DataStorage* GetData();


#ifdef SORTABLE_OBJECTS
	void SortObjectInArray(int objectIndex);
	void SortObjectArray();
#endif

private:
	//void UpdateObjects(float dt, unsigned int startIndex, unsigned int endIndex);
	//void UpdateObjects(Vector3f* p, Vector3f* m, Math::Matrix* rm, Vector3f* ra, float* rs, Vector3f* s, Math::Matrix* wm);

	void RenderGeometryPass();
	void RenderInstancedGeometryPass();
	void RenderLightPass();
	void RenderFinalPass();

#ifdef SORT_OBJECTS_BY_TEXTURE
	void SortObjectsInDataStorage();
#endif

	int m_viewProjBufferIndex;
	int m_lightBufferIndex;
	int m_matrixBufferIndex;
	int m_idBufferIndex;
	
	

	DataStorage m_dataStorage;

#ifdef SORTABLE_OBJECTS
	struct ObjectType
	{
		int textureIndex = -1;
		int modelIndex = -1;
		int orderInArray = -1;
		unsigned int nrOfThisType = 0U;
	};
	std::vector<ObjectType> m_objectTypes;
#endif

	ModelManager* m_modelManager;
	MaterialManager* m_materialManager;
	FrameWorkManager* m_frameWorkManager;
};

#endif