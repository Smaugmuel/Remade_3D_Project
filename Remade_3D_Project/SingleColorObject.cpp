#include "SingleColorObject.hpp"
#include "SingleColorModel.hpp"
#include <d3d11.h>

SingleColorObject::SingleColorObject()
{
}

SingleColorObject::~SingleColorObject()
{
}

bool SingleColorObject::Initialize(ID3D11Device* device, const char* modelName)
{
	//m_model = std::unique_ptr<Model>(ModelFactory::Create(modelName, device));

	m_model = std::make_unique<SingleColorModel>();

	if (!m_model->LoadFromFile(modelName))
	{
		return false;
	}
	if (!m_model->CreateVertexBuffer(device))
	{
		return false;
	}

	m_color = Vector3f(1, 0, 0);

	InitializeBaseClass();

	return true;
}

void SingleColorObject::Render(ID3D11DeviceContext* deviceContext)
{
	m_model->Render(deviceContext);
}

void SingleColorObject::SetColor(const Vector3f & color)
{
	m_color = color;
}

void SingleColorObject::SetColor(float r, float g, float b)
{
	SetColor(Vector3f(r, g, b));
}

const Vector3f & SingleColorObject::GetColor() const
{
	return m_color;
}