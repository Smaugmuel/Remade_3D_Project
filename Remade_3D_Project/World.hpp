#ifndef WORLD_HPP
#define WORLD_HPP
#include <memory>
#include "Singleton.hpp"

class FPSCounter;
class Scene;
class Character;

class World : public Singleton<World>
{
	friend class Singleton<World>;
	
	World();
	~World();

public:

	void Update(float dt);

	void SetScene(Scene* scene);

	Scene* GetScene();
	Character* GetPlayer();
	FPSCounter* GetFPSCounter();

private:

	Scene * m_scene;
	std::unique_ptr<Character> m_player;
	std::unique_ptr<FPSCounter> m_fpsCounter;
};

#endif