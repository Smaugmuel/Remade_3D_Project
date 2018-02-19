#ifndef GAME_HPP
#define GAME_HPP
#include "Singleton.hpp"
#include "SystemInformation.hpp"
#include <memory>
#include <vector>
#include <chrono>

class TextureObject;
class SingleColorObject;
class HUDObject;
class PointLight;
class FPSCounter;
class Character;

template<typename StateCategory> class StateMachine;

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
enum OrthogonalMode
{
	ORTHOGONAL_OFF,
	ORTHOGONAL_ON
};

class Game final : public Singleton<Game>
{
	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::time_point<std::chrono::steady_clock> Time;

	friend class Singleton<Game>;

	Game();
	~Game();

public:
	bool Initialize();
	void Run();

	bool ProcessInput();
	void Update(double dt);
	void Render();

private:
	void MapProjectionMatrix();

	//void CreatePickingVector(float x, float y);
	void CubeIntersection();

	void RenderNormal();
	void RenderDeferredFirstPass();
	void RenderDepth();
	void RenderShadowPass();
	void RenderMultipleShadowsPass();
	void RenderDeferredLightPass();
	void RenderDeferredLightMultipleShadowsPass();
	void RenderHUD();
	void RenderHUDText();


	std::vector<std::unique_ptr<TextureObject>> m_texturedCubes;
	std::unique_ptr<SingleColorObject> m_coloredFloor;
	std::unique_ptr<HUDObject> m_HUDObject;

	std::unique_ptr<PointLight> m_pointLights[MAX_NR_OF_LIGHTS];
	unsigned int m_nrOfLights;
	unsigned int m_currentLight;

	std::unique_ptr<FPSCounter> m_fpsCounter;

	RenderMode m_renderMode;
	HUDMode m_HUDMode;
	OrthogonalMode m_orthogonal;


	std::unique_ptr<Character> m_player;
};

#endif