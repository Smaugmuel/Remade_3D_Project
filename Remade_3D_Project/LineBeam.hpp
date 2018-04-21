#ifndef LINE_BEAM_HPP
#define LINE_BEAM_HPP
#include <vector>
#include "Vector3.hpp"

class LineObject;

class LineBeam
{
public:
	LineBeam();
	~LineBeam();

	bool Initialize(const Vector3f& position);

	bool AddLayer(unsigned int nrOfLinesOnLayer);

	void Update(float dt);

	void Render();

private:
	std::vector<std::vector<LineObject>> m_lines;
	Vector3f m_position;
};

#endif