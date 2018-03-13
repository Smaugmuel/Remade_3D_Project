#include "EditorState.hpp"

EditorState::EditorState(StateMachine<EditorState>* stateMachine) :
	m_stateMachine(stateMachine), m_selectedObject(nullptr)
{
}

EditorState::~EditorState()
{
}

void EditorState::SetSelectedObject(TextureObject * object)
{
	m_selectedObject = object;
}

TextureObject * EditorState::GetSelectedObject() const
{
	return m_selectedObject;
}
