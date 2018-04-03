#ifndef EDITOR_SCALE_STATE_HPP
#define EDITOR_SCALE_STATE_HPP
#include "EditorState.hpp"

class EditorScaleState : public EditorState
{
public:
	EditorScaleState();
	~EditorScaleState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;
	void RenderHUD() override;
};

#endif