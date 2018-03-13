#ifndef EDITOR_PLACEMENT_STATE_HPP
#define EDITOR_PLACEMENT_STATE_HPP
#include "EditorState.hpp"

class TextureObject;
class Scene;

class EditorPlacementState : public EditorState
{
public:
	EditorPlacementState(StateMachine<EditorState>* stateMachine, Scene* scene);
	~EditorPlacementState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;

private:
	Scene* m_scene;
	TextureObject* m_previewObject;
};

#endif