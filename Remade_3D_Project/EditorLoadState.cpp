#include "EditorLoadState.hpp"

#include "../Engine/Core/Engine.hpp"
#include "../Engine/Misc/StringConverter.hpp"
#include "../Engine/Events/EventDispatcher.hpp"

#include "TextButtonManager.hpp"

//#include "../Engine/GUI/GUIManager.hpp"
#include "../Engine/Misc/StringConverter.hpp"

#include "SceneStorage.hpp"
#include "Scene.hpp"


EditorLoadState::EditorLoadState() : EditorState::EditorState(), m_sceneButtonManager(nullptr)
{
}

EditorLoadState::~EditorLoadState()
{
	if (m_sceneButtonManager)
	{
		delete m_sceneButtonManager;
		m_sceneButtonManager = nullptr;
	}
}

bool EditorLoadState::Initialize()
{
	/* ============================================= Create the load icon ========================================== */
	//m_iconID = GUIManager::Get()->CreateImage("Icons/LoadIcon.png", Vector2i(0, 32));

	// Create menu and its buttons from the scenes found in the directory
	m_sceneButtonManager = new TextButtonManager<EditorLoadState>;
	CreateButtonsFromScenesInFolder();

	std::vector<TextMenuButton<EditorLoadState>> buttons;
	buttons = m_sceneButtonManager->RetrieveButtons();

	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		/*GUIManager::Get()->CreateText(
			StringConverter::ToString(buttons[i].GetText()).value(),
			buttons[i].GetAABA().center - buttons[i].GetAABA().halfSides
		);*/
	}

	return true;
}

void EditorLoadState::ProcessInput()
{
	//Input* input = Input::Get();

	/*if (input->IsKeyPressed(VK_LBUTTON))
	{
		m_sceneButtonManager->ProcessMouseClick(input->MousePosition());
	}

	if (input->IsKeyDown(VK_CONTROL))
	{
		if (input->IsKeyPressed('R'))
		{
			CreateButtonsFromScenesInFolder();
		}
	}*/
}

void EditorLoadState::Update(float dt)
{
}

void EditorLoadState::Render()
{
}

//void EditorLoadState::RenderHUD()
//{
//}

void EditorLoadState::CreateButtonsFromScenesInFolder()
{
	//GUIManager* gui_mngr = GUIManager::Get();
	unsigned int n = 0;

	/* ============================ Retrieve the names of all scenes in the directory ============================= */
	SceneStorage::Get()->UpdateExistingSceneNamesFromFolder();
	const std::vector<std::wstring>& wSceneNames = SceneStorage::Get()->GetExistingSceneNames();

	// Do nothing if no scenes exist
	n = wSceneNames.size();
	if (n == 0)
	{
		return;
	}

	/* ============================ Create buttons from the found scene names ============================= */
	m_sceneButtonManager->ClearButtons();

	for (unsigned int i = 0; i < n; i++)
	{
		// Calculate position and dimensions
		/*Vector2i halfDimensions = gui_mngr->GetDimensionsOfText(wSceneNames[i]) * 0.5;
		Vector2i position(halfDimensions.x, 96 + halfDimensions.y * (1 + i * 2));
		
		m_sceneButtonManager->CreateButton(
			this,
			&EditorLoadState::LoadScene,
			wSceneNames[i],
			position,
			halfDimensions
		);*/
	}
}

void EditorLoadState::LoadScene(const std::wstring & sceneName)
{
	SceneStorage* sceneStorage = SceneStorage::Get();
	EventDispatcher* eventDispatcher = EventDispatcher::Get();

	std::optional<std::string> str = StringConverter::ToString(sceneName);
	
	// Leave editor if string couldn't be converted
	if (!str)
	{
		eventDispatcher->Emit(Event(EventType::POP_GAMESTATE));
		return;
	}

	// Leave editor if scene failed to load
	if (!sceneStorage->LoadScene(str.value()))
	{
		eventDispatcher->Emit(Event(EventType::POP_GAMESTATE));
		return;
	}

	Scene* scene = sceneStorage->GetScene(str.value());

	// Do nothing if the chosen scene is the current one
	if (scene == m_scene)
	{
		return;
	}

	m_scene = scene;

	// Set up scene for rendering
	m_scene->LoadIntoRenderManager();

	// Notify of a scene change, and reset selection
	eventDispatcher->Emit(Event(EventType::SWITCHED_SCENE, m_scene));
	eventDispatcher->Emit(Event(EventType::SWITCHED_SELECTED_OBJECT, nullptr));
}