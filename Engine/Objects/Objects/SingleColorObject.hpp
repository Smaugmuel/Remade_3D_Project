#ifndef SINGLE_COLOR_OBJECT_HPP
#define SINGLE_COLOR_OBJECT_HPP
#include "Object.hpp"

class SingleColorObject final : public Object
{
public:
	SingleColorObject();
	~SingleColorObject();

	bool Initialize(const std::string& modelName, const Vector3f& color = Vector3f(1, 0, 0));

	void SetModel(const std::string& modelName);

	void SetColor(const Vector3f& color);
	void SetColor(float r, float g, float b);

	int GetModelIndex() const;
	const std::string& GetModelName() const;

	const Vector3f& GetColor() const;

private:
	void UpdateOBB() final override;

	int m_modelIndex;
	Vector3f m_color;
};

#endif