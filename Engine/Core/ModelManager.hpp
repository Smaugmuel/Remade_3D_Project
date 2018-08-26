#ifndef MODEL_MANAGER_HPP
#define MODEL_MANAGER_HPP
#include <map>
#include <vector>
#include <string>
#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"

class VertexBufferManager;
class MaterialManager;

struct TexturedModelVertex
{
	Vector3f position;
	Vector3f normal;
	Vector2f uv;
};
struct TexturedModel
{
	TexturedModelVertex* vertices = nullptr;
	unsigned int nrOfVertices = 0;
	int vertexBufferIndex = -1;
	int materialIndex = -1;
};

class ModelManager final
{
public:
	ModelManager();
	~ModelManager();

	bool Initialize(VertexBufferManager* vertexBufferManager, MaterialManager* materialManager);

	int LoadModel(const std::string& fileName);
	TexturedModel* GetModel(int index);

private:
	std::map<std::string, int> m_nameToIndexLinker;
	std::vector<TexturedModel> m_models;

	VertexBufferManager * m_vertexBufferManager;
	MaterialManager* m_materialManager;
};

#endif