#ifndef SINGLE_COLOR_OBJECT_HPP
#define SINGLE_COLOR_OBJECT_HPP
#include "Object.hpp"

class SingleColorObject : public Object
{
public:
	SingleColorObject();
	~SingleColorObject();

	bool Initialize() override;
	bool Initialize(const std::string& modelName, const Vector3f& color);

	void SetColor(const Vector3f& color);
	void SetColor(float r, float g, float b);

	const Vector3f& GetColor() const;

private:
	void UpdateOBB() override;

	Vector3f m_color;
};

#endif