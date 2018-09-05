#ifndef CONSTANT_BUFFER_MANAGER_HPP
#define CONSTANT_BUFFER_MANAGER_HPP
#include <vector>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class ConstantBufferManager final
{
public:
	ConstantBufferManager();
	~ConstantBufferManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	int CreateConstantBuffer(unsigned int bufferSize, void* initialData = nullptr);

	bool MapDataToBuffer(int index, void* bufferData);

	bool SetConstantBufferToVertexShader(int index, int slotInShader);
	bool SetConstantBufferToGeometryShader(int index, int slotInShader);
	bool SetConstantBufferToPixelShader(int index, int slotInShader);

private:
	struct Buffer
	{
		ID3D11Buffer* buffer = nullptr;
		unsigned int bufferSize = 0;
	};

	ID3D11Device * m_device;
	ID3D11DeviceContext* m_deviceContext;
	
	static const unsigned int MAX_NR_OF_BUFFERS = 4;
	Buffer m_buffers[MAX_NR_OF_BUFFERS];
	unsigned int m_nrOfBuffers;

	//std::vector<Buffer> m_buffers;
};

#endif