#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include "BufferStorage.hpp"

#include <vector>

/*

What needs to be created through this

Shaders
Buffers
	Vertex
	Constant
Textures
Matrices

*/

class FrameWork final
{
public:
	FrameWork();
	~FrameWork();

	int CreateVertexBuffer(unsigned int bufferSize, void* initialData = nullptr);
	int CreateConstantBuffer(unsigned int bufferSize, void* initialData = nullptr);
	//ID3D11Buffer* CreateDynamicVertexBuffer(const unsigned int& bufferSize, void* initialData);

private:
	BufferStorage m_buffers;
};

#endif