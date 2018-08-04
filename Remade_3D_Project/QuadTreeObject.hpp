#ifndef QUAD_TREE_OBJECT_HPP
#define QUAD_TREE_OBJECT_HPP
#include "../Engine/Math/AABA.hpp"

class QuadTreeNode;

class QuadTreeObject
{
protected:
	QuadTreeObject()
	{
		m_node = nullptr;
	}
	virtual ~QuadTreeObject()
	{
	}

public:
	AABA m_aaba;

	QuadTreeNode* m_node;
};

#endif