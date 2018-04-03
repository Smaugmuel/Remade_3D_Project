#include "QuadTree.hpp"
#include "QuadTreeNode.hpp"
#include "QuadTreeObject.hpp"

#include "HelperFunctions.hpp"

QuadTree::QuadTree()
{
	m_nodeIndices = nullptr;
	m_nodePool = nullptr;

	for (unsigned int i = 0; i < NR_OF_LEVELS; i++)
	{
		m_nodeLevels[i] = nullptr;
	}
}

QuadTree::~QuadTree()
{
	Reset();
}

void QuadTree::Create(AABB aabb)
{
	Create(aabb.center, aabb.halfSides);
}

void QuadTree::Create(Vector3f center, Vector3f halfSides)
{
	Reset();

	m_aabb.center = center;
	m_aabb.halfSides = halfSides;

	m_aaba.center = Vector2f(center.x, center.z);
	m_aaba.halfSides = Vector2f(halfSides.x, halfSides.z);

	m_nodeIndices = new unsigned int[NR_OF_NODES];

	//QuadTreeNode* nodes = Root();

	//for (unsigned int i = 0; i < NR_OF_NODES; i++)
	//{
	//	m_nodePool[i] = new QuadTreeNode;

	//	//nodes[i] = QuadTreeNode();
	//}
	m_nodePool = new QuadTreeNode[NR_OF_NODES];

	m_invHalfSides.x = 255 / (m_aaba.halfSides.x * 2);
	m_invHalfSides.y = 255 / (m_aaba.halfSides.y * 2);


	//QuadTreeNode* level = m_nodePool;
	QuadTreeNode* previousLevel = nullptr;
	
	unsigned int nodeOffset = 0;
	unsigned int nrOfNodesPerRow = 0;
	unsigned int nrOfNodesPerHalfRow = 0;
	unsigned int nrOfNodesPerLevel = 0;

	for (unsigned int i = 0; i < NR_OF_LEVELS; i++)
	{
		//m_nodeLevels[i] = level;

		// Set first node of level i
		m_nodeLevels[i] = &m_nodePool[nodeOffset];

		nrOfNodesPerRow = (1 << i);// pow(2, i);
		nrOfNodesPerHalfRow = (nrOfNodesPerRow >> 1);//nrOfNodesPerRow * 0.5;
		nrOfNodesPerLevel = nrOfNodesPerRow * nrOfNodesPerRow;

		//nodeOffset += nrOfNodesPerLevel;

		// Dimensions of nodes on level i
		Vector2f nodeHalfSides = m_aaba.halfSides * (1.0f / nrOfNodesPerRow);

		// Center position of first node on level i
		Vector2f levelStartPos = m_aaba.center - nodeHalfSides * static_cast<float>(nrOfNodesPerRow - 1);

		for (unsigned int y = 0; y < nrOfNodesPerRow; y++)
		{
			for (unsigned int x = 0; x < nrOfNodesPerRow; x++)
			{
				// Index of the current node
				unsigned int nodeIndex = nodeOffset + y * nrOfNodesPerRow + x;

				// Center position of the current node
				Vector2f nodeCenter = levelStartPos + Vector2f(nodeHalfSides.x * x, nodeHalfSides.y * y) * 2;

				m_nodePool[nodeIndex].SetAABA(nodeCenter, nodeHalfSides);

				// Set this node as a child of its parent, if it's not the root
				if (previousLevel)
				{
					// Find the index of parent node
					unsigned int parentIndexX = (x >> 1);//x * 0.5f;
					unsigned int parentIndexY = (y >> 1);//y * 0.5f;
					/*unsigned int parentIndex = parentIndexY * (1 << (i - 1)) + parentIndexX;
					unsigned int parentIndex = parentIndexY * pow(2, i - 1) + parentIndexX;*/
					unsigned int parentIndex = parentIndexY * nrOfNodesPerHalfRow + parentIndexX;

					// Find the index of this node from parent node [0, 3]
					unsigned int indexX = x & 1;
					unsigned int indexY = y & 1;
					unsigned int childIndex = indexY * 2 + indexX;

					// Set this node as a child to its parent at the correct index
					//previousLevel[parentIndex].SetChild(childIndex, &nodes[nodeIndex]);
					previousLevel[parentIndex].SetChild(childIndex, &m_nodePool[nodeIndex]);

					// per side: 4
					// 0, 1, 4, 5     => 0
					// 2, 3, 6, 7     => 1
					// 8, 9, 12, 13   => 2
					// 10, 11, 14, 15 => 3
					//
					// per side: 8
					// 0, 1, 8, 9       => 0
					// 2, 3, 10, 11     => 1
					// 4, 5, 12, 13     => 2
					// 6, 7, 14, 15     => 3
					// 16, 17, 24, 25   => 4
					// 18, 19, 26, 27   => 5
					// 20, 21, 28, 29   => 6
					// 22, 23, 30, 31   => 7
					// 32, 33, 40, 41   => 8
					// 34, 35, 42, 43   => 9
					// 36, 37, 44, 45   => 10
					// 38, 39, 46, 47   => 11
					// 48, 49, 56, 57   => 12
					// 50, 51, 58, 59   => 13
					// 52, 53, 60, 61   => 14
					// 54, 55, 62, 63   => 15
				}
			}
		}

		//previousLevel = level;
		previousLevel = m_nodeLevels[i];
		//level += nrOfNodesPerLevel;

		nodeOffset += nrOfNodesPerLevel;
	}
}

void QuadTree::Reset()
{
	if (m_nodeIndices)
	{
		//QuadTreeNode* nodes = Root();

		/*for (unsigned int i = 0; i < NR_OF_NODES; i++)
		{
			nodes[i].~QuadTreeNode();
		}
*/
		/*for (unsigned int i = 0; i < NR_OF_NODES; i++)
		{
			delete m_nodePool[i];
			m_nodePool[i] = nullptr;
		}*/

		delete[] m_nodeIndices;
		m_nodeIndices = nullptr;
	}
	if (m_nodePool)
	{
		delete[] m_nodePool;
		m_nodePool = nullptr;
	}
}

bool QuadTree::Insert(QuadTreeObject * object)
{
	const AABA& aaba = object->m_aaba;

	// If object isn't fully contained, add it to the root
	if (!m_aaba.Contains(aaba))
	{
		return Root()->AddObject(object);
	}

	// Get min- and max-bounds of AABA
	Vector2f minimum = aaba.center - aaba.halfSides;
	Vector2f maximum = aaba.center + aaba.halfSides;

	// Translate to a range between (0, 0) and (m_aaba.halfSides * 2)
	Vector2f shiftedMin = minimum - (m_aaba.center - m_aaba.halfSides);
	Vector2f shiftedMax = maximum - (m_aaba.center - m_aaba.halfSides);

	// Translate to a range between (0, 0) and (255, 255)
	shiftedMin.x *= m_invHalfSides.x;
	shiftedMin.y *= m_invHalfSides.y;
	shiftedMax.x *= m_invHalfSides.x;
	shiftedMax.y *= m_invHalfSides.y;

	// Round min down and max up
	int minX = static_cast<int>(shiftedMin.x);
	int minY = static_cast<int>(shiftedMin.y);
	int maxX = static_cast<int>(ceil(shiftedMax.x));
	int maxY = static_cast<int>(ceil(shiftedMax.y));

	// Clamp to [0, 255]
	minX = HF::Clamp(minX, 0, 255);
	minY = HF::Clamp(minY, 0, 255);
	maxX = HF::Clamp(maxX, 0, 255);
	maxY = HF::Clamp(maxY, 0, 255);

	/*
	Use binary XOR to determine the level of where to place the object
	Solution retrieved from http://lspiroengine.com/?p=530
	Solution presented in Game Programming Gems II by Matt Pritchard
	*/
	unsigned int x = minX ^ maxX;
	unsigned int y = minY ^ maxY;
	if (!x)
	{
		// All bits were the same, therefore min - max = 0 and can fit at the lowest level
		x = NR_OF_LEVELS - 1;
	}
	else
	{
		// Perform a bit hack to find the lowest level
		x = (NR_OF_LEVELS - 1) - HF::Clamp(static_cast<int>(HF::HighestBitIndex(x)), 0, NR_OF_LEVELS - 1);
	}
	if (!y)
	{
		// All bits were the same, therefore min - max = 0 and can fit at the lowest level
		y = NR_OF_LEVELS - 1;
	}
	else
	{
		// Perform a bit hack to find the lowest level
		y = (NR_OF_LEVELS - 1) - HF::Clamp(static_cast<int>(HF::HighestBitIndex(y)), 0, NR_OF_LEVELS - 1);
	}

	// Set level to the higher of the two
	unsigned int level = HF::Min(x, y);

	// Find which node to place the object in
	x = minX >> (NR_OF_LEVELS - level);
	y = minY >> (NR_OF_LEVELS - level);

	unsigned int yOffset = (1 << level);//pow(2U, level);

	// Place the object in the node
	return m_nodeLevels[level][y * yOffset + x].AddObject(object);
}

void QuadTree::RetrieveObjects(const AABB & aabb, std::list<QuadTreeObject*>& listToFill)
{
	// Check height intersections aswell
	if (!m_aabb.Intersects(aabb))
	{
		return;
	}

	AABA aaba;
	aaba.center = Vector2f(aabb.center.x, aabb.center.z);
	aaba.halfSides = Vector2f(aabb.halfSides.x, aabb.halfSides.z);

	RetrieveObjects(aaba, listToFill);
}

void QuadTree::RetrieveObjects(const AABA & aaba, std::list<QuadTreeObject*>& listToFill)
{
	Root()->RetrieveObjects(aaba, listToFill);
}

QuadTreeNode * QuadTree::Root()
{
	return &m_nodePool[0];
}