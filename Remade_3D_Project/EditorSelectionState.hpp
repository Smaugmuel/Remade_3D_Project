#ifndef EDITOR_SELECTION_STATE_HPP
#define EDITOR_SELECTION_STATE_HPP
#include "EditorState.hpp"
#include "Ray.hpp"

class Scene;

class EditorSelectionState : public EditorState
{
public:
	EditorSelectionState(StateMachine<EditorState>* stateMachine, Scene* scene);
	~EditorSelectionState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;

private:
	void SelectCube(const Ray& ray);

	Scene* m_scene;
};

#endif