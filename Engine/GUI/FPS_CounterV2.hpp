#ifndef FPS_COUNTER_V2_HPP
#define FPS_COUNTER_V2_HPP

class FPSCounterV2 final
{
public:
	FPSCounterV2();
	~FPSCounterV2();

	void Update(float dt);

	int CreateGUIText();
	void RemoveGUIText();

	void Show();
	void Hide();

	int GetGUIID() const;

private:
	int m_id;

	float m_timeElapsed;
	unsigned int m_nrOfUpdates;
};

#endif