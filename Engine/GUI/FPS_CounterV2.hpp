#ifndef FPS_COUNTER_V2_HPP
#define FPS_COUNTER_V2_HPP

class GUIManager;

class FPSCounterV2 final
{
public:
	FPSCounterV2();
	~FPSCounterV2();

	bool Initialize(GUIManager* guiManager);

	void Update(float dt);

	void Show();
	void Hide();

	int GetGUIID() const;

private:
	int m_id;

	float m_timeElapsed;
	unsigned int m_nrOfUpdates;

	GUIManager* m_guiManager;
};

#endif