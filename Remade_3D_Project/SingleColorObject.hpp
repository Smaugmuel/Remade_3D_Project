#ifndef SINGLE_COLOR_OBJECT_HPP
#define SINGLE_COLOR_OBJECT_HPP
#include "Object.hpp"
#include <DirectXMath.h>
#include "Vector3.hpp"
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;

class SingleColorModel;

class SingleColorObject : public Object
{
public:
	SingleColorObject();
	~SingleColorObject();

	bool Initialize(ID3D11Device* device, const char* modelName) override;
	void Render(ID3D11DeviceContext* deviceContext) override;

	void SetColor(const Vector3f& color);
	void SetColor(float r, float g, float b);

	const Vector3f& GetColor() const;

private:
	std::unique_ptr<SingleColorModel> m_model;
	Vector3f m_color;
};

#endif