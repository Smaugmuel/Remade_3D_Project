#ifndef LINE_MODEL_HPP
#define LINE_MODEL_HPP
#include "Model.hpp"

struct LineVertex
{
	Vector3f position;
	Vector3f color;
};

class LineModel final : public Model
{
public:
	LineModel();
	~LineModel();

	bool LoadFromFile(const char* fileName) final override;
	bool CreateVertexBuffer() final override;
	bool CreateOBB() final override;
	void SetupRender() final override;

private:
	LineVertex m_vertices[2];
};

#endif