#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include "BaseSystem.hpp"

class SystemManager;

template<typename Type>
class System : public BaseSystem
{
public:
	virtual ~System()
	{
	}

protected:
	System()
	{
	}

private:
	friend class SystemManager;

	/*
	Get the type of this system, represented as an unsigned int
	*/
	static SystemType GetType()
	{
		static SystemType type = m_type++;
		return type;
	}
};

#endif