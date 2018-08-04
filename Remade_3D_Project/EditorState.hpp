#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP
#include "../Engine/Events/EventReceiver.hpp"

//class HUDTexture;
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
	//virtual void RenderHUD();

protected:
	//bool InitializeIcon(const char* fileName);
	void ReceiveEvent(const Event& e) final override;

	int m_iconID;
	//HUDTexture* m_icon;
	TextureObject* m_selectedObject;
	Scene* m_scene;
};

#endif