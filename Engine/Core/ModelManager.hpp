#ifndef MODEL_MANAGER_HPP
#define MODEL_MANAGER_HPP
#include <map>
#include <string>
#include <memory>
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
	std::unique_ptr<TexturedModelVertex[]> vertices = nullptr;
	unsigned int nrOfVertices = 0;
	int vertexBufferIndex = -1;
	int materialIndex = -1;
};

static const unsigned int MAX_NR_OF_MODELS = 16U;

class ModelManager final
{
public:
	ModelManager();
	~ModelManager();

	bool Initialize(VertexBufferManager* vertexBufferManager, MaterialManager* materialManager);

	int LoadModel(const std::string& fileName);
	TexturedModel* GetModel(int index);

private:
	VertexBufferManager * m_vertexBufferManager;
	MaterialManager* m_materialManager;

	std::map<std::string, int> m_nameToIndexLinker;
	TexturedModel m_models[MAX_NR_OF_MODELS];
	unsigned int m_nrOfModels;
};

#endif