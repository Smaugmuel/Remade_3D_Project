#ifndef GAME_HPP
#define GAME_HPP
#include "Singleton.hpp"
#include <memory>
#include <vector>

class TextureObject;
class SingleColorObject;
class HUDObject;

enum RenderMode
{
	NORMAL_MODE,
	DEFERRED_MODE,
	DEPTH_MODE,
	NR_OF_RENDER_MODES
};
enum HUDMode
{
	HUD_OFF,
	TORGUE_HUD,
	DEFERRED_POSITIONS,
	DEFERRED_NORMALS,
	DEFERRED_COLORS,
	NR_OF_HUD_MODES
};
enum OrthogonalCamera
{
	ORTHOGONAL_OFF,
	ORTHOGONAL_ON
};

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
	void RenderHUD();

	std::vector<std::unique_ptr<TextureObject>> m_texturedCubes;
	std::unique_ptr<SingleColorObject> m_coloredFloor;
	std::unique_ptr<HUDObject> m_HUDObject;

	
	RenderMode m_renderMode;
	HUDMode m_HUDMode;
	OrthogonalCamera m_orthogonal;
};

#endif