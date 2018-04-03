#ifndef GAME_HPP
#define GAME_HPP
#include "Singleton.hpp"
#include "EventReceiver.hpp"

#include <chrono>

#include "StateMachine.hpp"
#include "GameState.hpp"

//class TextureObject;
//class SingleColorObject;
//class HUDObject;
//class PointLight;
//class FPSCounter;
//class Character;
////class QuadTree;

//enum RenderMode
//{
//	NORMAL_MODE,
//	DEFERRED_MODE,
//	DEFERRED_CHUNK_MODE,
//	DEFERRED_SPLIT_MODE,
//	DEFERRED_MULTIPLE_LIGHTS_MODE,
//	DEFERRED_MULTIPLE_SHADOW_LIGHTS_MODE,
//	DEPTH_MODE,
//	NR_OF_RENDER_MODES
//};
//enum HUDMode
//{
//	HUD_OFF,
//	TORGUE_HUD,
//	DEFERRED_POSITIONS,
//	DEFERRED_NORMALS,
//	DEFERRED_COLORS,
//	NR_OF_HUD_MODES
//};
//enum OrthogonalMode
//{
//	ORTHOGONAL_OFF,
//	ORTHOGONAL_ON
//};

class Game final : public Singleton<Game>, public EventReceiver
{
	typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::time_point<std::chrono::steady_clock> Time;

	friend class Singleton<Game>;

	Game();
	~Game();

public:
	bool Initialize();
	void Run();

	/*bool ProcessInput();
	void Update(float dt);
	void Render();*/

private:
	void ReceiveEvent(const Event& e) override;

	/*void MapProjectionMatrix();

	void CubeIntersection();

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
	void RenderHUD();
	void RenderHUDText();*/


	StateMachine<GameState> m_gameStateMachine;
	bool m_pop_game_state_flag;
	bool m_shutdown_flag;


	/*unsigned int m_nrOfCubes;
	TextureObject* m_texturedCubes;

	std::unique_ptr<SingleColorObject> m_coloredFloor;
	std::unique_ptr<HUDObject> m_HUDObject;

	std::unique_ptr<FPSCounter> m_fpsCounter;

	RenderMode m_renderMode;
	HUDMode m_HUDMode;
	OrthogonalMode m_orthogonal;

	//std::unique_ptr<QuadTree> m_quadTree;

	std::unique_ptr<Character> m_player;*/
};

#endif