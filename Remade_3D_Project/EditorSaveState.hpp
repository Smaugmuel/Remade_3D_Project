#ifndef EDITOR_SAVE_STATE_HPP
#define EDITOR_SAVE_STATE_HPP
#include "EditorState.hpp"
#include "Vector3.hpp"

class TextureObject;
class HUDObject;

class EditorSaveState : public EditorState
{
public:
	EditorSaveState();
	~EditorSaveState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;
	void RenderHUD() override;

private:
	HUDObject* m_saveIcon;
};

#endif