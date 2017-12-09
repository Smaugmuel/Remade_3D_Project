#ifndef GAME_HPP
#define GAME_HPP
#include "Singleton.hpp"
#include <memory>
#include <vector>

//class Camera;
class Object;

/* Added with rastertek */
class RenderTexture;
class DebugWindow;

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

	/* Added with rastertek */
	void RenderToTexture();

	void RenderDeferredFirstPass();
	void RenderDepth();
	void RenderShadowPass();
	void RenderDeferredLightPass();

	std::vector<std::unique_ptr<Object>> m_cubes;
	std::unique_ptr<Object> m_floor;
	
	/* Added with rastertek */
	std::unique_ptr<RenderTexture> m_renderTexture;
	std::unique_ptr<DebugWindow> m_debugWindow;
};

#endif