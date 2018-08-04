#ifndef QUAD_TREE_HPP
#define QUAD_TREE_HPP
#include "../Engine/Math/AABA.hpp"
#include "../Engine/Math/AABB.hpp"
#include <list>

#define NR_OF_LEVELS 8
#define NR_OF_NODES (1*1 + 2*2 + 4*4 + 8*8 + 16*16 + 32*32 + 64*64 + 128*128)

class QuadTreeNode;
class QuadTreeObject;

class QuadTree
{
public:
	QuadTree();
	~QuadTree();

	void Create(AABB aabb);
	void Create(Vector3f center, Vector3f halfSides);
	void Reset();

	bool Insert(QuadTreeObject* object);

	void RetrieveObjects(const AABB& aabb, std::list<QuadTreeObject*>& listToFill);
	void RetrieveObjects(const AABA& aaba, std::list<QuadTreeObject*>& listToFill);

	QuadTreeNode* Root();

private:
	unsigned int* m_nodeIndices;

	QuadTreeNode* m_nodePool;// [NR_OF_NODES];
	QuadTreeNode* m_nodeLevels[NR_OF_LEVELS];

	AABA m_aaba;
	AABB m_aabb;
	Vector2f m_invHalfSides;
};

#endif