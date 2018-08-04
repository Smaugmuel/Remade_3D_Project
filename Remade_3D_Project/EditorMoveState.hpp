#ifndef EDITOR_MOVE_STATE_HPP
#define EDITOR_MOVE_STATE_HPP
#include "EditorState.hpp"
#include "../Engine/Math/Vector3.hpp"

class EditorMoveState final : public EditorState
{
public:
	EditorMoveState();
	~EditorMoveState();

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;
	//void RenderHUD() final override;

private:
	void CheckCollisionsOfSelectedCube();


	Vector3f m_screenDirectionX;
	Vector3f m_screenDirectionY;

	float m_distanceMultiplier;
};

#endif