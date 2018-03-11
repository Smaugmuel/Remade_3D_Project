#ifndef SCENE_EDITOR_HPP
#define SCENE_EDITOR_HPP
#include "Singleton.hpp"
#include <vector>

class TextureObject;
class SingleColorObject;

class SceneEditor final : public Singleton<SceneEditor>
{
	friend class Singleton<SceneEditor>;
	SceneEditor();
	~SceneEditor();

public:
	void Update(float dt);

private:

	std::vector<TextureObject*> m_texturedObjects;
};

#endif