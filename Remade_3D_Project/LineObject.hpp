#ifndef LINE_OBJECT_HPP
#define LINE_OBJECT_HPP
#include "Object.hpp"

class LineObject : public Object
{
public:
	LineObject();
	~LineObject();

	bool Initialize() override;

private:
	void UpdateOBB() override;
};

#endif