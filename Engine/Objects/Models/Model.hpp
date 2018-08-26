#ifndef MODEL_HPP
#define MODEL_HPP
#include "../../Math/OBB.hpp"

struct ID3D11Buffer;
struct ID3D11DeviceContext;
class FrameWork;

class Model
{
protected:
	Model(FrameWork* frameWork);
public:
	virtual ~Model();

	virtual bool LoadFromFile(const char* fileName) = 0;
	virtual bool CreateVertexBuffer() = 0;
	virtual bool CreateOBB() = 0;
	virtual void SetupRender() = 0;
	void Render();

	const OBB& GetOBB() const;

protected:
	ID3D11Buffer* m_vertexBuffer;
	unsigned int m_nrOfVerts;

	OBB m_obb;

	FrameWork* m_frameWork;
};

#endif