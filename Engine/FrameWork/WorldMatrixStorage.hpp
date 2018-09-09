#ifndef WORLD_MATRIX_STORAGE_HPP
#define WORLD_MATRIX_STORAGE_HPP

struct Matrix;

struct MovedMatrixData
{
	int objectIndex = -1;
	int newMatrixIndex;
};

static const unsigned int MAX_NR_OF_OBJECTS = 16384;

class WorldMatrixStorage final
{
public:
	WorldMatrixStorage();
	~WorldMatrixStorage();

	bool Initialize();

	int ValidateMatrix(int objectIndex);

	/*
	Returns whichever object had its matrix moved, and the new index of the matrix
	*/
	MovedMatrixData InvalidateMatrixAndMoveObjectIndex(int matrixIndex);

private:
	Matrix* m_matrices;
	int* m_objectIndices;
	unsigned int m_nrOfValidMatrices;
};

#endif