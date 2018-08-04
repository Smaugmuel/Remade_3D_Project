#include "FrameWork.hpp"

FrameWork::FrameWork()
{
}

FrameWork::~FrameWork()
{
}

int FrameWork::CreateVertexBuffer(unsigned int bufferSize, void * initialData)
{
	return m_buffers.CreateVertexBuffer(bufferSize, initialData);
}

int FrameWork::CreateConstantBuffer(unsigned int bufferSize, void* initialData)
{
	return m_buffers.CreateConstantBuffer(bufferSize, initialData);
}