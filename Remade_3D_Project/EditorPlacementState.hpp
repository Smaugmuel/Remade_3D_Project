#ifndef EDITOR_PLACEMENT_STATE_HPP
#define EDITOR_PLACEMENT_STATE_HPP
#include "EditorState.hpp"

class TextureObject;

class EditorPlacementState final : public EditorState
{
public:
	EditorPlacementState();
	~EditorPlacementState();

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;
	//void RenderHUD() final override;

private:
	TextureObject * m_previewObject;
};

#endif