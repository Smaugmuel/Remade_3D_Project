#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP
#include "GameState.hpp"

#include <memory>

#include "RenderModes.hpp"
#include "HUDModes.hpp"
#include "OrthogonalModes.hpp"

class TextureObject;
class SingleColorObject;
class HUDObject;
class PointLight;
class FPSCounter;
class Character;
//class QuadTree;

class Scene;

class PlayState : public GameState
{
public:
	PlayState(StateMachine<GameState>* stateMachine);
	virtual ~PlayState();
	
	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;

private:
	void MapProjectionMatrix();

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
	void RenderHUDText();


	Scene* m_scene;

	//std::unique_ptr<TextureObject> m_ghostObject;

	/*unsigned int m_nrOfCubes;
	TextureObject* m_texturedCubes;

	std::unique_ptr<SingleColorObject> m_coloredFloor;*/
	std::unique_ptr<HUDObject> m_HUDObject;

	std::unique_ptr<FPSCounter> m_fpsCounter;

	RenderMode m_renderMode;
	HUDMode m_HUDMode;
	OrthogonalMode m_orthogonal;

	//std::unique_ptr<QuadTree> m_quadTree;

	std::unique_ptr<Character> m_player;
};

#endif