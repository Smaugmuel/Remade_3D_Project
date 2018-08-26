#ifndef SCENE_MANAGER_V3_HPP
#define SCENE_MANAGER_V3_HPP
#include "../Math/Matrix.hpp"
#include <vector>

class ModelManager;
class MaterialManager;
class FrameWorkManager;

struct ObjectV3
{
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;

	Matrix worldMatrix;

	int modelIndex = -1;
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

private:
	struct ViewProjectionMatrices
	{
		ViewProjectionMatrices(const Matrix& _view, const Matrix& _projection)
			: view(_view), projection(_projection)
		{
		}
		Matrix view;
		Matrix projection;
	};

	int m_viewProjBufferIndex;
	int m_worldBufferIndex;

	static const unsigned int MAX_NR_OF_OBJECTS = 20000;
	ObjectV3 m_objects[MAX_NR_OF_OBJECTS];
	unsigned int m_nrOfObjects;
	//std::vector<ObjectV3> m_objects;

	ModelManager* m_modelManager;
	MaterialManager* m_materialManager;
	FrameWorkManager* m_frameWorkManager;
};

#endif