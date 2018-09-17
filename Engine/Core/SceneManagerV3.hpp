#ifndef SCENE_MANAGER_V3_HPP
#define SCENE_MANAGER_V3_HPP
#include "DataStorage.hpp"

// The constant buffer size limit is 4096 float4's, which means 1024 float4x4's
#define NR_OF_MATRICES_IN_BUFFER 1024

// Because of the light buffer, "only" 2047 lights can exist
// 2048 lights could be stored, but that would leave no room for the ambient color and light counter
static const unsigned int MAX_NR_OF_LIGHTS = 100U;
static const unsigned int MAX_NR_OF_OBJECTS = MAX_NR_OF_DATA_INSTANCES;

//#define SORTABLE_OBJECTS

class ModelManager;
class MaterialManager;
class FrameWorkManager;

struct LightBuffer
{
	struct Light
	{
		Vector3f position;
		float dropoff = -0.01f;
		Vector3f diffuseColor;
		float padding = 0.0f;
	};

	Light lights[MAX_NR_OF_LIGHTS];
	Vector3f ambientColor;
	int nrOfLights;
};

class SceneManagerV3 final
{
public:
	SceneManagerV3();
	~SceneManagerV3();

	bool Initialize(ModelManager* modelManager, MaterialManager* materialManager, FrameWorkManager* frameWorkManager);

	void Render();

	void SetViewAndProjectionMatrices(const Matrix& view, const Matrix& projection);

	Object* CreateObject();
	//int CreateObject();
	DataStorage* GetObjectData();


#ifdef SORTABLE_OBJECTS
	void SortObjectInArray(int objectIndex);
	void SortObjectArray();
#endif

private:
	int m_viewProjBufferIndex;
	int m_lightBufferIndex;
	int m_matrixBufferIndex;
	int m_indexBufferIndex;

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