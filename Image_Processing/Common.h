#pragma once
class Common {
public:
	template<class T> 
	static T saturate_cast(T value, T min = 0.0, T max = 255.0)
	{
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}
};