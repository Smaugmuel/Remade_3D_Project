#ifndef SCENE_EDITOR_STATE_HPP
#define SCENE_EDITOR_STATE_HPP
#include "GameState.hpp"
#include <memory>

class Scene;
class TextureObject;
class Character;
class FPSCounter;

class SceneEditorState : public GameState
{
public:
	SceneEditorState(StateMachine<GameState>* stateMachine);
	virtual ~SceneEditorState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;

private:
	void MapProjectionMatrix();

	void CubeIntersection();

	void RenderNormal();
	void RenderHUDText();

	Scene* m_scene;
	std::unique_ptr<TextureObject> m_ghostObject;
	std::unique_ptr<Character> m_player;
	std::unique_ptr<FPSCounter> m_fpsCounter;
};

#endif