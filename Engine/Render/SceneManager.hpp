#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP
//#include "../Misc/Singleton.hpp"
#include "../Objects/Objects/TextureObject.hpp"
#include <vector>

class FrameWork;

enum class RenderModes
{
	NORMAL,
	DEFERRED_WITH_SHADOWS,
	DEFERRED_IN_CHUNKS,
	DEFERRED_SPLIT_TEXTURES,
	DEFERRED_WITH_MULTIPLE_LIGHTS,
	DEFERRED_WITH_MULTIPLE_LIGHTS_GIVING_SHADOW,
	DEPTH
};

enum class Textures
{
	TORGUE
};
enum class Models
{
	CUBE
};

struct SceneObject
{
	int id;
	TextureObject object;
};

class SceneManager //: public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	bool Initialize(FrameWork* frameWork);

	void Update(float dt);

	void Render();

	int AddObject(
		const std::string& modelName,
		Vector3f position = Vector3f(0,0,0),
		Vector3f rotation = Vector3f(0,0,0),
		Vector3f scale = Vector3f(1,1,1)
	);
	void SetObjectModel(int id, const std::string& modelName);
	void SetObjectTexture(int id, const std::string& textureName);
	void SetObjectPosition(int id, Vector3f position);
	void SetObjectRotation(int id, Vector3f rotation);
	void SetObjectScale(int id, Vector3f scale);

	void SetRenderMode(RenderModes mode);

private:
	void RenderNormal();
	void RenderDeferredFirstPass();
	void RenderDeferredFirstPassChunks();
	void RenderDepth();
	void RenderShadowPass();
	void RenderShadowPassChunks();
	void RenderMultipleShadowsPass();
	void RenderDeferredLightShadowPass();
	void RenderDeferredLightSplitPass();
	void RenderDeferredLightMultiplePass();
	void RenderDeferredLightMultipleShadowsPass();

	int GetIndexOfID(int id) const;

	RenderModes m_renderMode;

	//std::vector<std::vector<std::vector<std::vector<SceneObject>>>> m_objects;
	std::vector<SceneObject> m_objects;


	int m_vsViewMatID;
	int m_vsProjMatID;

	FrameWork* m_frameWork;
};

#endif