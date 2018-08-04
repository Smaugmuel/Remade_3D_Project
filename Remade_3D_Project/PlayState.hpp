#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP
#include "GameState.hpp"

#include <memory>

#include "RenderModes.hpp"
#include "HUDModes.hpp"
#include "OrthogonalModes.hpp"

class TextureObject;
class SingleColorObject;
//class HUDTexture;
class PointLight;
//class FPSCounter;
class Character;

//class QuadTree;

class Scene;

class PlayState final : public GameState
{
public:
	PlayState(StateMachineV2<GameState>* stateMachine);
	virtual ~PlayState();
	
	// Added with StateV2
	void OnEntry() final override;
	void OnExit() final override;

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;

private:
	void MapProjectionMatrix();

	void CubeIntersection();

	//void CollisionWithScene();

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
	/*void RenderHUD();*/
	void RenderHUDText();


	Scene* m_scene;

	//std::unique_ptr<TextureObject> m_ghostObject;

	/*unsigned int m_nrOfCubes;
	TextureObject* m_texturedCubes;

	std::unique_ptr<SingleColorObject> m_coloredFloor;*/
	//std::unique_ptr<HUDTexture> m_HUDTexture;

	//std::unique_ptr<FPSCounter> m_fpsCounter;

	RenderMode m_renderMode;
	HUDMode m_HUDMode;
	OrthogonalMode m_orthogonal;

	//std::unique_ptr<QuadTree> m_quadTree;

	std::unique_ptr<Character> m_player;
};

#endif