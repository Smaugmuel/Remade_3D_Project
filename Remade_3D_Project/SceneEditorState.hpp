#ifndef SCENE_EDITOR_STATE_HPP
#define SCENE_EDITOR_STATE_HPP
#include "GameState.hpp"
#include "EventReceiver.hpp"
#include <memory>

class Scene;
class EditorState;
class Character;
class FPSCounter;

class LineBeam;

enum class EditorModes
{
	SELECT,
	MOVE,
	ROTATE,
	SCALE,
	PLACE,
	SAVE,
	LOAD,
	NR_OF_EDITOR_MODES
};

class SceneEditorState : public GameState, public EventReceiver
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

	void RenderNormal();
	void RenderHUD();

	void ReceiveEvent(const Event& e) override;


	EditorModes m_editorMode;
	EditorState* m_editorStates[static_cast<unsigned int>(EditorModes::NR_OF_EDITOR_MODES)];

	Scene* m_scene;
	std::unique_ptr<Character> m_player;
	std::unique_ptr<FPSCounter> m_fpsCounter;

	LineBeam* m_beam;
};

#endif