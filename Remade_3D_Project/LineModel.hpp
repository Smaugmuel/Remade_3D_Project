#ifndef LINE_MODEL_HPP
#define LINE_MODEL_HPP
#include "Model.hpp"

struct LineVertex
{
	Vector3f position;
	Vector3f color;
};

class LineModel : public Model
{
public:
	LineModel();
	~LineModel();

	bool LoadFromFile(const char* fileName) override;
	bool CreateVertexBuffer(ID3D11Device* device) override;
	bool CreateOBB() override;
	void SetupRender(ID3D11DeviceContext* deviceContext) override;

private:
	LineVertex m_vertices[2];
};

#endif