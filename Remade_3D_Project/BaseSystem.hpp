#ifndef BASE_SYSTEM_HPP
#define BASE_SYSTEM_HPP

class BaseSystem
{
public:
	typedef unsigned int SystemType;

	virtual ~BaseSystem()
	{
	}

	virtual void Update(float dt) = 0;


	static SystemType m_type;

protected:
	BaseSystem()
	{
	}
};

#endif