#ifndef EDITOR_SAVE_STATE_HPP
#define EDITOR_SAVE_STATE_HPP
#include "EditorState.hpp"

class EditorSaveState final : public EditorState
{
public:
	EditorSaveState();
	~EditorSaveState();

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;
	//void RenderHUD() final override;
};

#endif