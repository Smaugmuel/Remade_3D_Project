#ifndef SCENE_MANAGER_V3_HPP
#define SCENE_MANAGER_V3_HPP
#include "../Math/Matrix.hpp"
#include <vector>

//#define USING_INSTANCING


class ModelManager;
class MaterialManager;
class FrameWorkManager;

//static const unsigned int MAX_NR_OF_MATRICES_IN_BUFFER = MAX_NR_OF_FLOAT4S_IN_BUFFER / 4;
//static const unsigned int MAX_NR_OF_OBJECTS_PER_BUFFER = MAX_NR_OF_MATRICES_IN_BUFFER;
static const unsigned int MAX_NR_OF_OBJECTS = 16384U;

// Because of the light buffer, "only" 4095 lights can exist, the remaining 32 bytes are occupied by the four ints
static const unsigned int MAX_NR_OF_LIGHTS = 4095U;

struct ObjectV3
{
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;

	Matrix worldMatrix;

	int modelIndex = -1;
};

struct Light
{
	Vector3f position;
	float dropoff = -0.01f;
};

struct LightBuffer
{
	Light lights[MAX_NR_OF_LIGHTS];
	int nrOfLights;
	int padding[3];
};

class SceneManagerV3 final
{
public:
	SceneManagerV3();
	~SceneManagerV3();

	bool Initialize(ModelManager* modelManager, MaterialManager* materialManager, FrameWorkManager* frameWorkManager);

	void Render();

	void SetViewAndProjectionMatrices(const Matrix& view, const Matrix& projection);

	int CreateObject();
	ObjectV3* GetObjectV3(int index);

	unsigned int GetNrOfObjects() const;

private:
	/*struct ViewProjectionMatrices
	{
		ViewProjectionMatrices(const Matrix& _view, const Matrix& _projection)
			: view(_view), projection(_projection)
		{
		}
		Matrix view;
		Matrix projection;
	};*/


	int m_viewProjBufferIndex;
	int m_worldBufferIndex;
	int m_lightBufferIndex;

#ifdef USING_INSTANCING
	struct MatrixBuffer
	{
		Matrix matrices[MAX_NR_OF_FLOAT4S_IN_BUFFER / 4];
	};
	struct InstanceBuffer
	{
		int instanceID;
		int padding[3];
	};
	int m_matrixBufferIndex;
	int m_instanceBufferIndex;
#endif

	ObjectV3* m_objects;
	unsigned int m_nrOfObjects;
	//std::vector<ObjectV3> m_objects;

	ModelManager* m_modelManager;
	MaterialManager* m_materialManager;
	FrameWorkManager* m_frameWorkManager;
};

#endif