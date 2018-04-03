#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP

struct BaseComponent
{
	typedef unsigned int ComponentType;

	virtual ~BaseComponent()
	{
	}

protected:
	BaseComponent()
	{
	}

	static ComponentType m_type;
};

#endif