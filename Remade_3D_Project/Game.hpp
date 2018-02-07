#ifndef GAME_HPP
#define GAME_HPP
#include "Singleton.hpp"
#include "SystemInformation.hpp"
#include <memory>
#include <vector>

class TextureObject;
class SingleColorObject;
class HUDObject;
class PointLight;

enum RenderMode
{
	NORMAL_MODE,
	DEFERRED_MODE,
	DEFERRED_SPLIT_MODE,
	DEFERRED_MULTIPLE_LIGHTS_MODE,
	DEFERRED_MULTIPLE_SHADOW_LIGHTS_MODE,
	DEPTH_MODE,
	NR_OF_RENDER_MODES
};
enum HUDMode
{
	HUD_OFF,
	TORGUE_HUD,
	DEFERRED_POSITIONS,
	DEFERRED_NORMALS,
	DEFERRED_COLORS,
	NR_OF_HUD_MODES
};
enum OrthogonalCamera
{
	ORTHOGONAL_OFF,
	ORTHOGONAL_ON
};

class Game final : public Singleton<Game>
{
	friend class Singleton<Game>;

	Game();
	~Game();

public:
	bool Initialize();
	void Run();

	bool ProcessInput();
	void Update();
	void Render();

private:
	void RenderNormal();
	void RenderDeferredFirstPass();
	void RenderDepth();
	void RenderShadowPass();
	void RenderMultipleShadowsPass();
	void RenderDeferredLightPass();
	void RenderDeferredLightMultipleShadowsPass();
	void RenderHUD();

	std::vector<std::unique_ptr<TextureObject>> m_texturedCubes;
	std::unique_ptr<SingleColorObject> m_coloredFloor;
	std::unique_ptr<HUDObject> m_HUDObject;

	std::unique_ptr<PointLight> m_pointLights[MAX_NR_OF_LIGHTS];
	unsigned int m_nrOfLights;
	unsigned int m_currentLight;

	RenderMode m_renderMode;
	HUDMode m_HUDMode;
	OrthogonalCamera m_orthogonal;
};

#endif