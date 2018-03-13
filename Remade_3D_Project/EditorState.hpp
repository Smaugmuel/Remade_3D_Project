#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

template<typename StateCategory> class StateMachine;

class TextureObject;

class EditorState
{
protected:
	EditorState(StateMachine<EditorState>* stateMachine);

public:
	virtual ~EditorState();

	virtual bool Initialize() = 0;
	virtual void ProcessInput() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	void SetSelectedObject(TextureObject* object);
	TextureObject* GetSelectedObject() const;

protected:
	StateMachine<EditorState>* m_stateMachine;
	TextureObject* m_selectedObject;
};

#endif