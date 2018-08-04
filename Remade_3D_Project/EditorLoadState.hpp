#ifndef EDITOR_LOAD_STATE_HPP
#define EDITOR_LOAD_STATE_HPP
#include "EditorState.hpp"
#include <string>

template<typename CallbackClassType, typename ... ParamTypes> class TextButtonManager;

class EditorLoadState final : public EditorState
{
public:
	EditorLoadState();
	~EditorLoadState();

	bool Initialize() final override;
	void ProcessInput() final override;
	void Update(float dt) final override;
	void Render() final override;
	//void RenderHUD() final override;

private:
	void CreateButtonsFromScenesInFolder();
	void LoadScene(const std::wstring& sceneName);

	TextButtonManager<EditorLoadState>* m_sceneButtonManager;
};

#endif