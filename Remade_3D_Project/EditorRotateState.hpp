#ifndef EDITOR_ROTATE_STATE_HPP
#define EDITOR_ROTATE_STATE_HPP
#include "EditorState.hpp"
#include "../Engine/Math/Vector3.hpp"

class EditorRotateState final : public EditorState
{
public:
	EditorRotateState();
	~EditorRotateState();

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;
	//void RenderHUD() final override;

private:
	Vector3f m_screenDirectionX;
	Vector3f m_screenDirectionY;

	//HUDTexture* m_rotateIcon;
};

#endif