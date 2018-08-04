#ifndef GAME_HPP
#define GAME_HPP
#include "../Engine/Misc/Singleton.hpp"
#include "../Engine/Misc/StateMachines/StateMachineV2.hpp"
#include "../Engine/Events/EventReceiver.hpp"

#include "GameState.hpp"

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
	friend class Singleton<Game>;

	Game();
	~Game();

public:
	bool Initialize();
	void Run();

private:
	void ReceiveEvent(const Event& e) final override;

	/*void RenderNormal();
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

	StateMachineV2<GameState> m_gameStateMachine;
	bool m_pop_game_state_flag;
	bool m_shutdown_flag;

	
	/*RenderMode m_renderMode;
	HUDMode m_HUDMode;
	OrthogonalMode m_orthogonal;*/
};

#endif