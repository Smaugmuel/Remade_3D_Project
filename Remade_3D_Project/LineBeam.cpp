#include "LineBeam.hpp"

// For rendering the lines
#include "ShaderManager.hpp"
#include "LineModel.hpp"
#include "ModelStorage.hpp"
#include "ConstantBufferStorage.hpp"
#include "Direct3D.hpp"
#include "LineObject.hpp"

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
	m_lines.push_back(std::vector<LineObject>());

	std::vector<LineObject>* lines = &m_lines.back();

	for (unsigned int i = 0; i < nrOfLinesOnLayer; i++)
	{
		lines->push_back(LineObject());

		if (!(*lines)[i].Initialize())
			return false;

		(*lines)[i].SetScale(10, 0, 0);
	}
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

		float angleOfLayer = powf(-1, layer) * angle;
		float angleOffset = 2 * 3.14159265f / lines->size();
		float distanceFromCenter = layer * 0.02f;

		for (unsigned int lineOnLayer = 0; lineOnLayer < lines->size(); lineOnLayer++)
		{
			float angleOfLine = angleOfLayer + angleOffset * lineOnLayer;
			Vector3f positionOffset = Vector3f(0, cosf(angleOfLine), sinf(angleOfLine)) * (distanceFromCenter * (sinf(angle) + 1.0f) * 0.5f);
			
			LineObject* line = &(*lines)[lineOnLayer];
			line->SetPosition(m_position + positionOffset);
			line->Update();
		}
	}
}

void LineBeam::Render()
{
	ID3D11DeviceContext* deviceContext = Direct3D::Get()->GetDeviceContext();
	ConstantBufferStorage* bufferStorage = ConstantBufferStorage::Get();

	ShaderManager::Get()->SetShaderType(deviceContext, ShaderType::LINE);

	LineModel* lineModel = ModelStorage::Get()->GetLineModel();
	lineModel->SetupRender(deviceContext);

	for (unsigned int layer = 0; layer < m_lines.size(); layer++)
	{
		for (unsigned int lineOnLayer = 0; lineOnLayer < m_lines[layer].size(); lineOnLayer++)
		{
			bufferStorage->SetVSWorldMatrix(deviceContext, m_lines[layer][lineOnLayer].GetWorldMatrix());
			lineModel->Render(deviceContext);
		}
	}
}
