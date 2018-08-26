#include "LineBeam.hpp"

#include "../Engine/Render/Shaders/ShaderManager.hpp"
#include "../Engine/FrameWork/Direct3D.hpp"
#include "../Engine/Objects/Models/LineModel.hpp"
#include "../Engine/Objects/Models/ModelStorage.hpp"
#include "../Engine/Objects/Objects/LineObject.hpp"
#include "../Engine/Buffers/ConstantBufferStorage.hpp"

LineBeam::LineBeam()
{
	
}

LineBeam::~LineBeam()
{
}

bool LineBeam::Initialize(const Vector3f & position)
{
	m_position = position;

	if (!AddLayer(1))
		return false;
	if (!AddLayer(8))
		return false;
	if (!AddLayer(8))
		return false;
	if (!AddLayer(8))
		return false;

	return true;
}

bool LineBeam::AddLayer(unsigned int nrOfLinesOnLayer)
{
	unsigned int currentLayer = m_lines.size();

	m_lines.push_back(std::vector<LineObject>());

	std::vector<LineObject>* lines = &m_lines.back();

	for (unsigned int i = 0; i < nrOfLinesOnLayer; i++)
	{
		lines->push_back(LineObject());

		LineObject* line = &lines->back();

		if (!line->Initialize())
			return false;

		line->SetScale(10, 1, 1);
		
		if (currentLayer < 2)
		{
			line->SetColor(Vector3f(1, 1, 1));
		}
	}

	return true;
}

void LineBeam::Update(float dt)
{
	static float angle = 0.0f;
	angle += dt;

	if (angle > 2 * 3.14159265f)
	{
		angle -= 2 * 3.14159265f;
	}

	for (unsigned int layer = 0; layer < m_lines.size(); layer++)
	{
		std::vector<LineObject>* lines = &m_lines[layer];

		float angleOfLayer = powf(-1.0f, static_cast<float>(layer)) * angle;
		float angleOffset = 2 * 3.14159265f / lines->size();
		float distanceFromCenter = layer * 0.02f;

		for (unsigned int lineOnLayer = 0; lineOnLayer < lines->size(); lineOnLayer++)
		{
			float angleOfLine = angleOfLayer + angleOffset * lineOnLayer;
			Vector3f positionOffset = Vector3f(0, cosf(angleOfLine), sinf(angleOfLine)) * (distanceFromCenter/* * (sinf(angle) + 1.0f) * 0.5f*/);
			
			LineObject* line = &(*lines)[lineOnLayer];
			line->SetPosition(m_position + positionOffset);
			line->Rotate(Vector3f(dt * 0.1f, 0, 0));
			line->Update();
		}
	}
}

void LineBeam::Render()
{
	/*ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::LINE);

	LineModel* lineModel = ModelStorage::Get()->GetLineModel();
	lineModel->SetupRender();

	for (unsigned int layer = 0; layer < m_lines.size(); layer++)
	{
		std::vector<LineObject>* lines = &m_lines[layer];
		for (unsigned int lineOnLayer = 0; lineOnLayer < lines->size(); lineOnLayer++)
		{
			LineObject* line = &(*lines)[lineOnLayer];

			bufferStorage->SetVSWorldMatrix(deviceContext, line->GetWorldMatrix());
			bufferStorage->SetVSColor(deviceContext, line->GetColor());
			lineModel->Render();
		}
	}*/
}
