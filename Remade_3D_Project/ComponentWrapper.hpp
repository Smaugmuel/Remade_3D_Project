#ifndef COMPONENT_WRAPPER_HPP
#define COMPONENT_WRAPPER_HPP

typedef unsigned int EntityID;

template<typename Type>
class ComponentWrapper
{
public:
	ComponentWrapper(EntityID entity)
	{
		m_entity = entity;
	}
	~ComponentWrapper()
	{
	}

private:
	EntityID m_entity;
};

#endif