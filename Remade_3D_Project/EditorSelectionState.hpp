#ifndef EDITOR_SELECTION_STATE_HPP
#define EDITOR_SELECTION_STATE_HPP
#include "EditorState.hpp"
#include "../Engine/Math/Ray.hpp"

class EditorSelectionState final : public EditorState
{
public:
	EditorSelectionState();
	~EditorSelectionState();

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;
	//void RenderHUD() final override;

private:
	void SelectCube(const Ray& ray);
};

#endif