#include "WorldMatrixStorage.hpp"
#include "../Math/Matrix.hpp"
#include <cstring>			// For memcpy

WorldMatrixStorage::WorldMatrixStorage() : m_matrices(nullptr), m_objectIndices(nullptr)
{
}

WorldMatrixStorage::~WorldMatrixStorage()
{
}

bool WorldMatrixStorage::Initialize()
{
	m_matrices = new Matrix[MAX_NR_OF_OBJECTS];
	if (!m_matrices)
		return false;

	m_objectIndices = new int[MAX_NR_OF_OBJECTS];
	if (!m_objectIndices)
		return false;

	return true;
}

int WorldMatrixStorage::ValidateMatrix(int objectIndex)
{
	int matrixIndex = -1;
	if (m_nrOfValidMatrices < MAX_NR_OF_OBJECTS)
	{
		matrixIndex = m_nrOfValidMatrices;
		m_objectIndices[matrixIndex] = objectIndex;
		m_nrOfValidMatrices++;
	}
	return matrixIndex;
}

MovedMatrixData WorldMatrixStorage::InvalidateMatrixAndMoveObjectIndex(int matrixIndex)
{
	MovedMatrixData movedMatrixData;

	if (matrixIndex >= 0 && matrixIndex < m_nrOfValidMatrices)
	{
		/*
		Move the data of the last matrix to the invalidated matrix
		*/
		std::memcpy(&m_matrices[matrixIndex], &m_matrices[m_nrOfValidMatrices - 1], sizeof(Matrix));

		/*
		Change the index of the last matrix to the index of the invalidated matrix
		*/
		m_objectIndices[m_nrOfValidMatrices - 1] = matrixIndex;

		/*
		The moved matrix belonged to the last object
		*/
		movedMatrixData.objectIndex = m_nrOfValidMatrices - 1;
		movedMatrixData.newMatrixIndex = matrixIndex;

		m_nrOfValidMatrices--;
	}

	return movedMatrixData;
}
