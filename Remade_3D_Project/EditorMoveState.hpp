#ifndef EDITOR_MOVE_STATE_HPP
#define EDITOR_MOVE_STATE_HPP
#include "EditorState.hpp"

class TextureObject;

class EditorMoveState : public EditorState
{
public:
	EditorMoveState(StateMachine<EditorState>* stateMachine);
	~EditorMoveState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;

private:
	bool m_mouseLeftDown;
	float m_objectDistance;
};

#endif