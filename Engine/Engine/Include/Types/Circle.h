#pragma once
#include "Vec2.h"

template<typename T>
class Circle_ {
public:
	Vec2_<T> center;
	T radius;
public:
	Circle_() {}
	~Circle_() {}
	Circle_(T _x, T _y, T _radius)
		: 
		center(_x, _y), radius(_radius)
	{}
	Circle_(const Vec2_<T> &_center, T _radius)
		: 
		center(_center), radius(_radius)
	{}

};

typedef Circle_<float> CircleF;
typedef Circle_<int> CircleI;