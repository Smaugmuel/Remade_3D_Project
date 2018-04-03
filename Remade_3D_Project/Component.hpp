#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include "BaseComponent.hpp"
#include "ComponentWrapper.hpp"

template<typename Type>
struct Component : public BaseComponent
{
	static ComponentType GetType()
	{
		static ComponentType type = m_type++;
		return type;
	}
};

#endif