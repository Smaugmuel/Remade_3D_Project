#ifndef DATA_STORAGE_HPP
#define DATA_STORAGE_HPP
#include "../Math/Vector3.hpp"
#include "../Math/Matrix.hpp"
#include <memory>

#define MAX_NR_OF_DATA_INSTANCES 8192U

struct Object
{
	int index = -1;
	Vector3f* positionPtr = nullptr;
	Vector3f* scalePtr = nullptr;
	Matrix* rotationMatrixPtr = nullptr;
	Matrix* worldMatrixPtr = nullptr;
	int* modelIndexPtr = nullptr;
	int* textureIndexPtr = nullptr;
};

class DataStorage
{
public:
	DataStorage() : m_nrOfObjects(0)
	{
		m_data = std::make_unique<Data>();

	}
	~DataStorage()
	{
	}

	Object* CreateObject()
	{
		if (m_nrOfObjects == MAX_NR_OF_DATA_INSTANCES)
		{
			return nullptr;
		}

		Data* data = m_data.get();
		Object* object = &data->objects[m_nrOfObjects];
		object->index = m_nrOfObjects;
		object->modelIndexPtr =		&data->modelIndices[m_nrOfObjects];
		object->textureIndexPtr =	&data->textureIndices[m_nrOfObjects];
		object->positionPtr =		&data->positions[m_nrOfObjects];
		object->scalePtr =			&data->scales[m_nrOfObjects];
		object->rotationMatrixPtr =	&data->rotationMatrices[m_nrOfObjects];
		object->worldMatrixPtr =	&data->worldMatrices[m_nrOfObjects];
		
		m_nrOfObjects++;

		return object;
	}

	bool RemoveObject(int index)
	{
		if (index < 0 || index >= static_cast<int>(m_nrOfObjects))
			return false;


		Data* data = m_data.get();
		Object* objectToMove = &data->objects[m_nrOfObjects - 1];

		// Move data
		data->positions[index] = data->positions[m_nrOfObjects - 1];
		data->scales[index] = data->scales[m_nrOfObjects - 1];
		data->rotationMatrices[index] = data->rotationMatrices[m_nrOfObjects - 1];
		data->worldMatrices[index] = data->worldMatrices[m_nrOfObjects - 1];
		data->modelIndices[index] = data->modelIndices[m_nrOfObjects - 1];
		data->textureIndices[index] = data->textureIndices[m_nrOfObjects - 1];

		// Move pointers of object to their moved data
		objectToMove->positionPtr = &data->positions[index];
		objectToMove->scalePtr = &data->scales[index];
		objectToMove->rotationMatrixPtr = &data->rotationMatrices[index];
		objectToMove->worldMatrixPtr = &data->worldMatrices[index];
		objectToMove->modelIndexPtr = &data->modelIndices[index];
		objectToMove->textureIndexPtr = &data->textureIndices[index];

		// Change object index
		objectToMove->index = index;

		m_nrOfObjects--;

		return true;
	}

	Vector3f* GetPositions()
	{
		return m_data->positions;
	}
	Vector3f* GetScales()
	{
		return m_data->scales;
	}
	Matrix* GetRotationMatrices()
	{
		return m_data->rotationMatrices;
	}
	Matrix* GetWorldMatrices()
	{
		return m_data->worldMatrices;
	}
	int* GetModelIndices()
	{
		return m_data->modelIndices;
	}
	int* GetTextureIndices()
	{
		return m_data->textureIndices;
	}

	Object* GetObjectPtr(int index)
	{
		return index < 0 || index >= static_cast<int>(m_nrOfObjects) ? nullptr : &m_data->objects[index];
	}

	unsigned int GetNrOfObjects() const noexcept
	{
		return m_nrOfObjects;
	}

private:
	struct Data
	{
		Vector3f positions[MAX_NR_OF_DATA_INSTANCES];
		Vector3f scales[MAX_NR_OF_DATA_INSTANCES];
		Matrix rotationMatrices[MAX_NR_OF_DATA_INSTANCES];
		Matrix worldMatrices[MAX_NR_OF_DATA_INSTANCES];
		int modelIndices[MAX_NR_OF_DATA_INSTANCES];
		int textureIndices[MAX_NR_OF_DATA_INSTANCES];
		Object objects[MAX_NR_OF_DATA_INSTANCES];
	};

	std::unique_ptr<Data> m_data;

	unsigned int m_nrOfObjects;
};

#endif
