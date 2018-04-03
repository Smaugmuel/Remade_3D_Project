#ifndef EDITOR_MOVE_STATE_HPP
#define EDITOR_MOVE_STATE_HPP
#include "EditorState.hpp"
#include "Vector3.hpp"

class HUDObject;

class EditorMoveState : public EditorState
{
public:
	EditorMoveState();
	~EditorMoveState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;
	void RenderHUD() override;

private:
	Vector3f m_screenDirectionX;
	Vector3f m_screenDirectionY;

	float m_distanceMultiplier;

	HUDObject* m_moveIcon;
};

#endif