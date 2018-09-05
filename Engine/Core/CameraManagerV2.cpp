#include "CameraManagerV2.hpp"

CameraManagerV2::CameraManagerV2()
{
}

CameraManagerV2::~CameraManagerV2()
{
}

int CameraManagerV2::CreateCamera()
{
	m_cameras.push_back(CameraV3());
	return m_cameras.size() - 1;
}

CameraV3 * CameraManagerV2::GetCamera(int index)
{
	return index >= 0 && index < static_cast<int>(m_cameras.size()) ? &m_cameras[index] : nullptr;
}

unsigned int CameraManagerV2::NrOfCameras() const
{
	return m_cameras.size();
}
