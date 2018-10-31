#ifndef DATA_STORAGE_HPP
#define DATA_STORAGE_HPP
//#include "../Math/Vector3.hpp"
//#include "../Math/Matrix.hpp"
#include "../Math/Math.hpp"
#include <memory>

// Because of the light buffer, "only" 2047 lights can exist
// 2048 lights could be stored, but that would leave no room for the ambient color and light counter
#define MAX_NR_OF_LIGHT_INSTANCES 2047U
//#define MAX_NR_OF_OBJECT_INSTANCES 8192U
#define MAX_NR_OF_OBJECT_INSTANCES 262144U

struct ObjectV4
{
	int index = -1;
	Vector3f* positionPtr = nullptr;
	Vector3f* scalePtr = nullptr;
	Math::Matrix* rotationMatrixPtr = nullptr;
	Math::Matrix* worldMatrixPtr = nullptr;
	int* modelIndexPtr = nullptr;
	int* textureIndexPtr = nullptr;

	Vector3f* movementPtr = nullptr;
	Vector3f* rotationAxisPtr = nullptr;
	float* rotationSpeedPtr = nullptr;
};
struct Light
{
	int index = -1;
	Vector3f* positionPtr = nullptr;
	Vector3f* diffuseColorPtr = nullptr;
	float* dropoffPtr = nullptr;
};
struct ObjectData
{
	Vector3f positions[MAX_NR_OF_OBJECT_INSTANCES];
	Vector3f scales[MAX_NR_OF_OBJECT_INSTANCES];
	Math::Matrix rotationMatrices[MAX_NR_OF_OBJECT_INSTANCES];
	Math::Matrix worldMatrices[MAX_NR_OF_OBJECT_INSTANCES];
	int modelIndices[MAX_NR_OF_OBJECT_INSTANCES];
	int textureIndices[MAX_NR_OF_OBJECT_INSTANCES];

	// Movements
	Vector3f movements[MAX_NR_OF_OBJECT_INSTANCES];
	Vector3f rotationAxises[MAX_NR_OF_OBJECT_INSTANCES];
	float rotationSpeeds[MAX_NR_OF_OBJECT_INSTANCES];

	ObjectV4 objects[MAX_NR_OF_OBJECT_INSTANCES];
};
struct LightData
{
	Vector3f positions[MAX_NR_OF_LIGHT_INSTANCES];
	Vector3f diffuseColors[MAX_NR_OF_LIGHT_INSTANCES];
	float dropoffs[MAX_NR_OF_LIGHT_INSTANCES];
	Light lights[MAX_NR_OF_LIGHT_INSTANCES];
	Vector3f ambientColor;
};

class DataStorage
{
public:
	DataStorage()
	{
		m_data = std::make_unique<Data>();

	}
	~DataStorage()
	{
	}

	ObjectV4* CreateObject()
	{
		Data* data = m_data.get();
		if (data->nrOfObjects == MAX_NR_OF_OBJECT_INSTANCES)
		{
			return nullptr;
		}

		ObjectV4* object = &data->objectData.objects[data->nrOfObjects];
		object->index = data->nrOfObjects;
		object->positionPtr =		&data->objectData.positions[data->nrOfObjects];
		object->scalePtr =			&data->objectData.scales[data->nrOfObjects];
		object->rotationMatrixPtr =	&data->objectData.rotationMatrices[data->nrOfObjects];
		object->worldMatrixPtr =	&data->objectData.worldMatrices[data->nrOfObjects];
		object->modelIndexPtr =		&data->objectData.modelIndices[data->nrOfObjects];
		object->textureIndexPtr =	&data->objectData.textureIndices[data->nrOfObjects];
		
		object->movementPtr = &data->objectData.movements[data->nrOfObjects];
		object->rotationAxisPtr = &data->objectData.rotationAxises[data->nrOfObjects];
		object->rotationSpeedPtr = &data->objectData.rotationSpeeds[data->nrOfObjects];

		data->nrOfObjects++;

		return object;
	}
	Light* CreateLight()
	{
		Data* data = m_data.get();
		if (data->nrOfLights == MAX_NR_OF_LIGHT_INSTANCES)
		{
			return nullptr;
		}

		Light* light = &data->lightData.lights[data->nrOfLights];
		light->index = data->nrOfLights;
		light->positionPtr = &data->lightData.positions[data->nrOfLights];
		light->diffuseColorPtr = &data->lightData.diffuseColors[data->nrOfLights];
		light->dropoffPtr = &data->lightData.dropoffs[data->nrOfLights];

		data->nrOfLights++;

		return light;
	}

	bool RemoveObject(int index)
	{
		Data* data = m_data.get();
		if (index < 0 || index >= static_cast<int>(data->nrOfObjects))
			return false;

		ObjectV4* objectToMove = &data->objectData.objects[data->nrOfObjects - 1];

		// Move data
		data->objectData.positions[index] = data->objectData.positions[data->nrOfObjects - 1];
		data->objectData.scales[index] = data->objectData.scales[data->nrOfObjects - 1];
		data->objectData.rotationMatrices[index] = data->objectData.rotationMatrices[data->nrOfObjects - 1];
		data->objectData.worldMatrices[index] = data->objectData.worldMatrices[data->nrOfObjects - 1];
		data->objectData.modelIndices[index] = data->objectData.modelIndices[data->nrOfObjects - 1];
		data->objectData.textureIndices[index] = data->objectData.textureIndices[data->nrOfObjects - 1];
		data->objectData.movements[index] = data->objectData.movements[data->nrOfObjects - 1];
		data->objectData.rotationAxises[index] = data->objectData.rotationAxises[data->nrOfObjects - 1];
		data->objectData.rotationSpeeds[index] = data->objectData.rotationSpeeds[data->nrOfObjects - 1];

		// Move pointers of object to their moved data
		objectToMove->positionPtr = &data->objectData.positions[index];
		objectToMove->scalePtr = &data->objectData.scales[index];
		objectToMove->rotationMatrixPtr = &data->objectData.rotationMatrices[index];
		objectToMove->worldMatrixPtr = &data->objectData.worldMatrices[index];
		objectToMove->modelIndexPtr = &data->objectData.modelIndices[index];
		objectToMove->textureIndexPtr = &data->objectData.textureIndices[index];
		objectToMove->movementPtr = &data->objectData.movements[index];
		objectToMove->rotationAxisPtr = &data->objectData.rotationAxises[index];
		objectToMove->rotationSpeedPtr = &data->objectData.rotationSpeeds[index];

		// Change object index
		objectToMove->index = index;

		data->nrOfObjects--;

		return true;
	}
	bool RemoveLight(int index)
	{
		Data* data = m_data.get();
		if (index < 0 || index >= static_cast<int>(data->nrOfLights))
			return false;

		Light* lightToMove = &data->lightData.lights[data->nrOfLights - 1];

		// Move data
		data->lightData.positions[index] = data->lightData.positions[data->nrOfLights - 1];
		data->lightData.diffuseColors[index] = data->lightData.diffuseColors[data->nrOfLights - 1];
		data->lightData.dropoffs[index] = data->lightData.dropoffs[data->nrOfLights - 1];

		// Move pointers of light to their moved data
		lightToMove->positionPtr = &data->lightData.positions[index];
		lightToMove->diffuseColorPtr = &data->lightData.diffuseColors[index];
		lightToMove->dropoffPtr = &data->lightData.dropoffs[index];

		// Change light index
		lightToMove->index = index;

		data->nrOfLights--;

		return true;
	}

	ObjectData* GetObjectData()
	{
		return &m_data->objectData;
	}
	LightData* GetLightData()
	{
		return &m_data->lightData;
	}

	ObjectV4* GetObjectPtr(int index)
	{
		return index < 0 || index >= static_cast<int>(m_data->nrOfObjects) ? nullptr : &m_data->objectData.objects[index];
	}
	Light* GetLightPtr(int index)
	{
		return index < 0 || index >= static_cast<int>(m_data->nrOfLights) ? nullptr : &m_data->lightData.lights[index];
	}

	unsigned int GetNrOfObjects() const
	{
		return m_data->nrOfObjects;
	}
	unsigned int GetNrOfLights() const
	{
		return m_data->nrOfLights;
	}

private:
	struct Data
	{
		ObjectData objectData;
		LightData lightData;
		unsigned int nrOfObjects = 0U;
		unsigned int nrOfLights = 0U;
	};

	std::unique_ptr<Data> m_data;
};

#endif
