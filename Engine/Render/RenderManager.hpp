#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP
#include "../Misc/Singleton.hpp"
#include "../Events/EventReceiver.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class TextureObject;
class SingleColorObject;

class RenderManager final : public Singleton<RenderManager>, public EventReceiver
{
	friend class Singleton<RenderManager>;

	// Having nested unordered maps directly caused two warnings about name exceeding length
	// These warnings did not occur with normal maps
	// Using structs as containers prevents these warning

	struct PerTexturedObjectVector
	{
		std::vector<TextureObject*> elements;
	};
	struct PerTexturedModelMap
	{
		std::unordered_map<std::string, PerTexturedObjectVector> elements;
	};
	struct PerTexturedTextureMap
	{
		std::unordered_map<std::string, PerTexturedModelMap> elements;
	};

	struct PerSingleColoredObjectVector
	{
		std::vector<SingleColorObject*> elements;
	};
	struct PerSingleColoredModelMap
	{
		std::unordered_map<std::string, PerSingleColoredObjectVector> elements;
	};

	RenderManager();
	~RenderManager();

public:
	void Reset();

	void RenderTexturedObjects();
	void RenderSingleColoredObjects();

	void AddTexturedObject(TextureObject* obj);
	void AddSingleColoredObject(SingleColorObject* obj);
	
	void ChangeTexture(TextureObject* obj, std::string newTextureName);

private:

	void ReceiveEvent(const Event& e) final override;

	PerTexturedTextureMap m_texturedObjects;
	PerSingleColoredModelMap m_singleColoredObjects;
};

#endif