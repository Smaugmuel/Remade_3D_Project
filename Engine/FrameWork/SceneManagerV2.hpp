#ifndef SCENE_MANAGER_V2_HPP
#define SCENE_MANAGER_V2_HPP
#include "ObjectV2.hpp"
#include "ReusableStorage.hpp"

class TextureManager;
class MatrixManager;

class SceneManagerV2 final
{
public:
	SceneManagerV2();
	~SceneManagerV2();

	bool Initialize(TextureManager* textureManager, MatrixManager* matrixManager);

	int CreateObject();
	void RemoveObject(int id);
	void SetObject(int id, const TexturedObjectV2& object);
	void SetObjectTexture(int id, const std::string& textureName);
	
	TexturedObjectV2* GetObject(int id);

private:
	ReusableStorage<TexturedObjectV2> m_objects;

	TextureManager* m_textureManager;
	MatrixManager* m_matrixManager;
};

#endif