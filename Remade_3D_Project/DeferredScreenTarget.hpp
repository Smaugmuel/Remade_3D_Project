#ifndef DEFERRED_SCREEN_TARGET_HPP
#define DEFERRED_SCREEN_TARGET_HPP
#include "Singleton.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class DeferredScreenTarget final : public Singleton<DeferredScreenTarget>
{
	friend class Singleton<DeferredScreenTarget>;

	struct Vertex
	{
		Vector3f position;
		Vector2f uv;
	};

	DeferredScreenTarget();
	~DeferredScreenTarget();

public:
	bool Initialize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext);

private:
	Vertex m_vertices[3];
	ID3D11Buffer* m_vertexBuffer;
};

#endif