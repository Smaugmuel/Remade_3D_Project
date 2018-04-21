#ifndef SCENE_HPP
#define SCENE_HPP
#include <vector>
#include <string>
#include "Vector3.hpp"
#include "ObjectAmountSettings.hpp"

class TextureObject;
class SingleColorObject;

class Scene
{
	struct ObjectData
	{
		Vector3f p;
		Vector3f r;
		Vector3f s;
		std::string m;
	};

	struct TexturedObjectData : public ObjectData
	{
		std::string t;
	};

	struct SingleColoredObjectData : public ObjectData
	{
		Vector3f c;
	};

public:
	Scene();
	virtual ~Scene();

	void Update(float dt);

	void LoadIntoRenderManager();

	void AddTexturedObject(const std::string& modelName, /*const std::string& textureName,*/ const Vector3f& position, const Vector3f& rotation, const Vector3f& scale);
	void AddSingleColoredObject(const std::string& modelName, const Vector3f& color, const Vector3f& position, const Vector3f& rotation, const Vector3f& scale);

	bool RemoveTexturedObject(TextureObject* object);
	bool RemoveSingleColoredObject(SingleColorObject* object);

	bool LoadFromFile(std::string fileName);
	void SaveToFile(std::string fileName);

	void Reset();

	TextureObject** GetTexturedObjects();
	SingleColorObject** GetSingleColoredObjects();

	unsigned int GetNrOfTexturedObjects() const;
	unsigned int GetNrOfSingleColoredObjects() const;

	const std::string& GetName() const;

private:
	/*void RenderTexturedObjects();
	void RenderSingleColoredObjects();*/


	unsigned int m_nrOfTexturedObjects;
	unsigned int m_nrOfSingleColoredObjects;

	TextureObject* m_texturedObjects[MAX_NR_OF_TEXTURED_OBJECTS];
	SingleColorObject* m_singleColoredObjects[MAX_NR_OF_SINGLE_COLORED_OBJECTS];

	std::string m_sceneName;
};

#endif