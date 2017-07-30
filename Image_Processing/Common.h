#pragma once
#include <vector>
#include <algorithm>
using std::vector;

#pragma region 自定义的常量
const double pi = 3.1415926535898; //尽量以const替换#define 
const double eps = 0.000001;
#pragma endregion


class Common {
public:
	template<class T>
	static T saturate_cast(const T value, const T min = 0.0, const T max = 255.0)
	{
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}

	template <class T>
	static void Norm( vector<T> &vecT, T normMin, T normMax) //这里两个变量不能为const,后面lambda表达式不知道怎么接收const的参数
	{
		T max = *std::max_element(vecT.begin(), vecT.end());
		T min = *std::min_element(vecT.begin(), vecT.end());

		std::for_each(vecT.begin(), vecT.end(), [max, min,normMax,normMin](T &val) {
			val = (val - min) / (max - min)*(normMax - normMin) + normMin;
		});

	}
};

