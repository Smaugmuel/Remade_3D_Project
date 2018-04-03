#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP
#include "EventReceiver.hpp"

class TextureObject;
class Scene;

class EditorState : public EventReceiver
{
protected:
	EditorState();

public:
	virtual ~EditorState();

	virtual bool Initialize() = 0;
	virtual void ProcessInput() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void RenderHUD() = 0;

protected:
	void ReceiveEvent(const Event& e) override;

	TextureObject* m_selectedObject;
	Scene* m_scene;
};

#endif