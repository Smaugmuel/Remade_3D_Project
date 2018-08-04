#ifndef BUFFER_STORAGE_HPP
#define BUFFER_STORAGE_HPP
#include <vector>

struct ID3D11Buffer;

class BufferStorage final
{
public:
	BufferStorage();
	~BufferStorage();

	int CreateVertexBuffer(unsigned int bufferSize, void* initialData = nullptr);
	int CreateConstantBuffer(unsigned int bufferSize, void* initialData = nullptr);

private:
	std::vector<ID3D11Buffer*> m_vertexBuffers;
	std::vector<ID3D11Buffer*> m_constantBuffers;
};

#endif