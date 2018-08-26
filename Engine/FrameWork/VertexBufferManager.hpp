#ifndef VERTEX_BUFFER_MANAGER_HPP
#define VERTEX_BUFFER_MANAGER_HPP
#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

class VertexBufferManager final
{
public:
	VertexBufferManager();
	~VertexBufferManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	int CreateBuffer(unsigned int bufferSize, unsigned int vertexSize, void* initialData);
	bool SetBufferToInputAssembler(int index, int slotInShader);

private:
	struct Buffer
	{
		ID3D11Buffer* buffer = nullptr;
		unsigned int vertexSize = 0;
	};

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	std::vector<Buffer> m_buffers;
};

#endif