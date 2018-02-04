#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP
#include "HUDText.hpp"

class FPS_Counter : public HUDText
{
public:
	FPS_Counter();
	~FPS_Counter();

	void Update(double dt);

private:
	double m_timeElapsed;
	unsigned int m_nrOfUpdates;
};

#endif