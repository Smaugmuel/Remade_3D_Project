#ifndef EDITOR_SELECTION_STATE_HPP
#define EDITOR_SELECTION_STATE_HPP
#include "EditorState.hpp"
#include "Ray.hpp"

class EditorSelectionState : public EditorState
{
public:
	EditorSelectionState();
	~EditorSelectionState();

	bool Initialize() override;
	void ProcessInput() override;
	void Update(float dt) override;
	void Render() override;
	void RenderHUD() override;

private:
	void SelectCube(const Ray& ray);
};

#endif