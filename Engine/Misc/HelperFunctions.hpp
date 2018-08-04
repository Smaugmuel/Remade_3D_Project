#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

namespace HF
{
	template<typename T> T Clamp(T val, T low, T high)
	{
		return (val < low ? low : (val > high ? high : val));
	}

	template<typename T> T Max(T val1, T val2)
	{
		return val1 > val2 ? val1 : val2;
	}

	template<typename T> T Min(T val1, T val2)
	{
		return val1 < val2 ? val1 : val2;
	}

	template<typename T> void Swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	unsigned int HighestBitIndex(unsigned int val);
}

#endif