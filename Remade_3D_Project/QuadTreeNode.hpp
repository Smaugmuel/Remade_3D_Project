#ifndef QUAD_TREE_NODE_HPP
#define QUAD_TREE_NODE_HPP
#include <list>
#include "AABA.hpp"
#include "Vector2.hpp"

class QuadTreeObject;

class QuadTreeNode
{
public:
	QuadTreeNode();
	~QuadTreeNode();

	void SetAABA(const AABA& aaba);
	void SetAABA(const Vector2f& center, const Vector2f& halfSides);

	void SetChild(unsigned int index, QuadTreeNode* child);

	bool AddObject(QuadTreeObject* object);
	void RemoveObject(QuadTreeObject* object);

	void RetrieveObjects(const AABA& aaba, std::list<QuadTreeObject*>& listToFill);

private:
	QuadTreeNode * m_children[4];
	AABA m_aaba;

	std::list<QuadTreeObject*> m_objects;
};

#endif