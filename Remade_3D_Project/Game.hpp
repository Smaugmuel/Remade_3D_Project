#ifndef GAME_HPP
#define GAME_HPP
#include "Singleton.hpp"
#include <memory>
#include <vector>

//class Camera;
class Object;

class Game final : public Singleton<Game>
{
	friend class Singleton<Game>;

	Game();
	~Game();

public:
	bool Initialize();
	void Run();

private:
	//std::unique_ptr<Camera> m_playerCamera;
	std::vector<std::unique_ptr<Object>> m_objects;

};

#endif