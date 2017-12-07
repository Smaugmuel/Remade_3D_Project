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

	bool ProcessInput();
	void Update();
	void Render();

private:
	void RenderNormal();
	void RenderDeferredFirstPass();
	void RenderDepth();
	void RenderShadowPass();
	void RenderDeferredLightPass();

	std::vector<std::unique_ptr<Object>> m_cubes;
	std::unique_ptr<Object> m_floor;
};

#endif