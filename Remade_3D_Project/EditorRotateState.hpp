#ifndef EDITOR_ROTATE_STATE_HPP
#define EDITOR_ROTATE_STATE_HPP
#include "EditorState.hpp"
#include "Vector3.hpp"

class TextureObject;
class HUDObject;

class EditorRotateState : public EditorState
{
public:
	EditorRotateState();
	~EditorRotateState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;
	void RenderHUD() override;

private:
	Vector3f m_screenDirectionX;
	Vector3f m_screenDirectionY;

	HUDObject* m_rotateIcon;
};

#endif