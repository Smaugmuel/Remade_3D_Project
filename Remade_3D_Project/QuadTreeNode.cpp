#include "QuadTreeNode.hpp"
#include "QuadTreeObject.hpp"

QuadTreeNode::QuadTreeNode()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		m_children[i] = nullptr;
	}
}

QuadTreeNode::~QuadTreeNode()
{
}

void QuadTreeNode::SetAABA(const AABA & aaba)
{
	m_aaba = aaba;
}

void QuadTreeNode::SetAABA(const Vector2f & center, const Vector2f & halfSides)
{
	m_aaba.center = center;
	m_aaba.halfSides = halfSides;
}

void QuadTreeNode::SetChild(unsigned int index, QuadTreeNode * child)
{
	m_children[index] = child;
}

bool QuadTreeNode::AddObject(QuadTreeObject * object)
{
	if (!m_aaba.Contains(object->m_aaba))
	{
		if (m_aaba.Contains(object->m_aaba))
			return true;

		return false;
	}

	m_objects.push_back(object);
	object->m_node = this;

	return true;
}

void QuadTreeNode::RemoveObject(QuadTreeObject * object)
{
	m_objects.remove(object);
}

void QuadTreeNode::RetrieveObjects(const AABA & aaba, std::list<QuadTreeObject*>& listToFill)
{
	if (!aaba.Intersects(m_aaba))
		return;

	if (!m_objects.empty())
	{
		listToFill.insert(listToFill.end(), m_objects.begin(), m_objects.end());
	}

	if (!m_children[0])
		return;

	m_children[0]->RetrieveObjects(aaba, listToFill);
	m_children[1]->RetrieveObjects(aaba, listToFill);
	m_children[2]->RetrieveObjects(aaba, listToFill);
	m_children[3]->RetrieveObjects(aaba, listToFill);
}