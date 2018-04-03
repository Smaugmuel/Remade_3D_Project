#ifndef EVENT_TYPES_HPP
#define EVENT_TYPES_HPP

enum class EventType
{
	// Managing game states
	POP_GAMESTATE,
	FAILED_TO_INITIALIZE,

	// Managing main menu
	PRESSED_PLAY_BUTTON,
	PRESSED_EDITOR_BUTTON,

	// Managing editor states
	SWITCHED_SCENE,
	SWITCHED_SELECTED_OBJECT,

	// Managing entities
	CREATED_ENTITY,
	REMOVED_ENTITY,
	ATTACHED_COMPONENT,
	DETACHED_COMPONENT
};

#endif