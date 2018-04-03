#ifndef EDITOR_PLACEMENT_STATE_HPP
#define EDITOR_PLACEMENT_STATE_HPP
#include "EditorState.hpp"

class TextureObject;

class EditorPlacementState : public EditorState
{
public:
	EditorPlacementState();
	~EditorPlacementState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;
	void RenderHUD() override;

private:
	TextureObject * m_previewObject;
};

#endif