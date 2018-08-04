#ifndef SCENE_EDITOR_STATE_HPP
#define SCENE_EDITOR_STATE_HPP
#include "GameState.hpp"
#include "../Engine/Events/EventReceiver.hpp"
#include <memory>

class Scene;
class EditorState;
class Character;
//class FPSCounter;

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

class SceneEditorState final : public GameState, public EventReceiver
{
public:
	SceneEditorState(StateMachineV2<GameState>* stateMachine);
	virtual ~SceneEditorState();

	void OnEntry() final override;
	void OnExit() final override;

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;

private:
	void MapProjectionMatrix();

	void RenderNormal();
	//void RenderHUD();

	void ReceiveEvent(const Event& e) final override;


	EditorModes m_editorMode;
	EditorState* m_editorStates[static_cast<unsigned int>(EditorModes::NR_OF_EDITOR_MODES)];

	Scene* m_scene;
	std::unique_ptr<Character> m_player;
	//std::unique_ptr<FPSCounter> m_fpsCounter;

	LineBeam* m_beam;
};

#endif