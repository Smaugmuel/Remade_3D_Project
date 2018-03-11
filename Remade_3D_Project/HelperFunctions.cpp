#include "HelperFunctions.hpp"

unsigned int HF::HighestBitIndex(unsigned int val)
{
	int i = -1;

	if (val)
	{
		while (val > 0)
		{
			val >>= 1;
			i++;
		}
	}
	else
	{
		i = 0;
	}

	return static_cast<unsigned int>(i);
}