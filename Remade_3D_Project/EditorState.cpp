#include "EditorState.hpp"
#include "EventDispatcher.hpp"

EditorState::EditorState() : m_selectedObject(nullptr), m_scene(nullptr)
{
	EventDispatcher::Get()->Subscribe(EventType::SWITCHED_SCENE, this);
	EventDispatcher::Get()->Subscribe(EventType::SWITCHED_SELECTED_OBJECT, this);
}

EditorState::~EditorState()
{
	EventDispatcher::Get()->Unsubscribe(EventType::SWITCHED_SCENE, this);
	EventDispatcher::Get()->Unsubscribe(EventType::SWITCHED_SELECTED_OBJECT, this);
}

void EditorState::ReceiveEvent(const Event & e)
{
	switch (e.type)
	{
	case EventType::SWITCHED_SCENE:
		m_scene = static_cast<Scene*>(e.value);
		break;
	case EventType::SWITCHED_SELECTED_OBJECT:
		m_selectedObject = static_cast<TextureObject*>(e.value);
		break;
	default:
		break;
	}
}
