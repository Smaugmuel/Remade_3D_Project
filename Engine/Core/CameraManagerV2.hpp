#ifndef CAMERA_MANAGER_V2_HPP
#define CAMERA_MANAGER_V2_HPP
#include "CameraV3.hpp"
#include <vector>

class CameraManagerV2 final
{
public:
	CameraManagerV2();
	~CameraManagerV2();

	int CreateCamera();
	CameraV3* GetCamera(int index);
	unsigned int NrOfCameras() const;

private:
	std::vector<CameraV3> m_cameras;
};

#endif