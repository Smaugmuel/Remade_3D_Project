#include "EditorState.hpp"

#include "../Engine/Events/EventDispatcher.hpp"

// For the icon
//#include "../Engine/GUI/HUDTexture.hpp"
//#include "../Engine/Render/Direct3D.hpp"
//#include "../Engine/Render/Shaders/ShaderManager.hpp"

EditorState::EditorState() : /*m_icon(nullptr),*/ m_selectedObject(nullptr), m_scene(nullptr)
{
	EventDispatcher::Get()->Subscribe(EventType::SWITCHED_SCENE, this);
	EventDispatcher::Get()->Subscribe(EventType::SWITCHED_SELECTED_OBJECT, this);
}

EditorState::~EditorState()
{
	/*if (m_icon)
	{
		delete m_icon;
		m_icon = nullptr;
	}*/

	EventDispatcher::Get()->Unsubscribe(EventType::SWITCHED_SCENE, this);
	EventDispatcher::Get()->Unsubscribe(EventType::SWITCHED_SELECTED_OBJECT, this);
}

//bool EditorState::InitializeIcon(const char * fileName)
//{
//	/*m_icon = new HUDTexture;
//	if (!m_icon->Initialize(Direct3D::Get()->GetDevice(), fileName, Vector2i(300, 300), Vector2i(32, 32)))
//	{
//		return false;
//	}
//	m_icon->SetPosition(Vector2i(0, 64));
//	m_icon->SetDimensions(Vector2i(32, 32));*/
//
//	return true;
//}

//void EditorState::RenderHUD()
//{
//	/*Direct3D* d3d = Direct3D::Get();
//	ShaderManager* shaderManager = ShaderManager::Get();
//	ID3D11DeviceContext* deviceContext = d3d->GetDeviceContext();
//
//	ID3D11ShaderResourceView* iconTexture = m_icon->GetShaderResourceView();
//	deviceContext->PSSetShaderResources(0, 1, &iconTexture);
//
//	shaderManager->SetShaderType(deviceContext, ShaderType::HUD);
//
//	m_icon->Render(d3d->GetDeviceContext());*/
//}

void EditorState::ReceiveEvent(const Event & e)
{
	switch (e.type)
	{
	case EventType::SWITCHED_SCENE:
		m_scene = static_cast<Scene*>(e.value);
		break;
	case EventType::SWITCHED_SELECTED_OBJECT:
		m_selectedObject = static_cast<TextureObject*>(e.value);
		break;
	default:
		break;
	}
}
