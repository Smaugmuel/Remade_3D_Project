#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP
#include "Singleton.hpp"
#include "EventReceiver.hpp"
#include <vector>
#include <map>
#include <string>

class TextureObject;
class SingleColorObject;

class RenderManager : public Singleton<RenderManager>, public EventReceiver
{
	friend class Singleton<RenderManager>;

	// To do:
	// 1 Set up texture shader
	// 1.1 Set up textures
	// 1.1.1 Set up models
	// 1.1.1.1 Render each object of this texture and model
	// 2 Set up single color shader
	// 2.1 Set up models
	// 2.1.1 Render each object of this model


	typedef std::vector<TextureObject*> PerTexturedObjectVector;
	typedef std::map<std::string, PerTexturedObjectVector> PerTexturedModelMap;
	typedef std::map<std::string, PerTexturedModelMap> PerTexturedTextureMap;
	typedef std::map<std::string, PerTexturedTextureMap> PerTexturedShaderMap;

	typedef std::vector<SingleColorObject*> PerSingleColoredObjectVector;
	typedef std::map<std::string, PerSingleColoredObjectVector> PerSingleColoredModelMap;
	typedef std::map<std::string, PerSingleColoredObjectVector> PerSingleColoredShaderMap;


	RenderManager();
	~RenderManager();

public:

	void Render();


private:
	void ReceiveEvent(const Event& e) override;

};

#endif