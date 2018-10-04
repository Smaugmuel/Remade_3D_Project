#ifndef OBJECT_V3_HPP
#define OBJECT_V3_HPP
#include "../Math/Vector3.hpp"
#include "../Math/Matrix.hpp"

// Sometimes a forward declared class can be a friend class
// I'm unsure of why it works now but not always
class SceneManagerV3;

class ObjectV3
{
public:
	void SetPosition(Vector3f position);
	void SetScale(Vector3f scale);
	void SetModel(int modelIndex);
	void SetTexture(int textureIndex);

	void Rotate(Vector3f axis, float angle);
	void Rotate(const Math::Matrix& rotationMatrix);

	Vector3f GetPosition() const noexcept;
	Vector3f GetScale() const noexcept;

	int GetObjectIndex() const noexcept;
	int GetModelIndex() const noexcept;
	int GetTextureIndex() const noexcept;

	const Math::Matrix& GetWorldMatrix() const noexcept;

private:
	friend class SceneManagerV3;

	ObjectV3();
	~ObjectV3();

	Vector3f m_position;
	Vector3f m_scale;

	int m_index = -1;
	int m_modelIndex = -1;
	int m_textureIndex = -1;

	Math::Matrix m_translationMatrix;
	Math::Matrix m_rotationMatrix;
	Math::Matrix m_scaleMatrix;
	
	mutable bool m_isWorldMatrixUpdated;
	mutable Math::Matrix m_worldMatrix;

	SceneManagerV3* m_sceneManager;
};

#endif