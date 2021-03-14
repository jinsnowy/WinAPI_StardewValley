#pragma once
#include "Types/Vec2.h"
#include <cmath>
namespace Math
{
	template<typename T>
	inline float Distance(const Vec2_<T>& tPos1, const Vec2_<T>& tPos2)
	{
		T x = tPos1.x - tPos2.x;
		T y = tPos1.y - tPos2.y;

		return sqrtf(x * x + y * y);
	}
};

