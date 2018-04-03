#ifndef COMPONENT_TABLE_HPP
#define COMPONENT_TABLE_HPP
#include <vector>
#include <algorithm>

typedef unsigned int EntityID;

template<typename Type>
class ComponentTable
{
public:
	typedef std::vector<Type>::iterator Iterator;

	ComponentTable()
	{

	}
	~ComponentTable()
	{

	}

	Iterator Begin()
	{
		return m_components.begin();
	}
	Iterator End()
	{
		return m_components.end();
	}
	Iterator Find(EntityID entity)
	{
		return std::find(m_components.begin(), m_components.end(), entity);
	}
	Type& Get(EntityID entity)
	{
		Iterator it = Find(entity);
		return *it;
	}
	void AddComponent(const Type& component)
	{
		m_components.push_back(component);
	}
	void DeleteComponent(EntityID entity)
	{
		
	}

private:

	std::vector<Type> m_components;
};


#endif