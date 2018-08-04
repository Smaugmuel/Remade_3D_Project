#ifndef EDITOR_SCALE_STATE_HPP
#define EDITOR_SCALE_STATE_HPP
#include "EditorState.hpp"

class EditorScaleState final : public EditorState
{
public:
	EditorScaleState();
	~EditorScaleState();

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;
	//void RenderHUD() final override;
};

#endif