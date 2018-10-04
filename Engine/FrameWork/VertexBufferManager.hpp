#ifndef VERTEX_BUFFER_MANAGER_HPP
#define VERTEX_BUFFER_MANAGER_HPP
#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

static const unsigned int MAX_NR_OF_VERTEX_BUFFERS = 16U;

class VertexBufferManager final
{
public:
	VertexBufferManager();
	~VertexBufferManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	int CreateBuffer(unsigned int bufferSize, unsigned int vertexSize, void* initialData);
	bool SetBufferToInputAssembler(int index, int slotInShader);

private:
	ID3D11Buffer * m_buffers[MAX_NR_OF_VERTEX_BUFFERS] = { nullptr };
	unsigned int m_vertexSizes[MAX_NR_OF_VERTEX_BUFFERS] = { 0U };

	unsigned int m_nrOfBuffers;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};

#endif