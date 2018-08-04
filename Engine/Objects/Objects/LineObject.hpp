#ifndef LINE_OBJECT_HPP
#define LINE_OBJECT_HPP
#include "Object.hpp"

class LineObject final : public Object
{
public:
	LineObject();
	~LineObject();

	bool Initialize() final override;

	void SetColor(const Vector3f& color);
	const Vector3f& GetColor() const;

private:
	void UpdateOBB() final override;

	Vector3f m_color;
};

#endif