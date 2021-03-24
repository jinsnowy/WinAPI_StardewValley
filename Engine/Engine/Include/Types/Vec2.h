#pragma once
#include <cmath>

template <typename T>
class Vec2_
{
public:
	Vec2_() = default;
	Vec2_(T x_in, T y_in)
		:
		x(x_in),
		y(y_in)
	{}
	Vec2_(const Vec2_& rhs) 
	{
		*this = rhs;
	}
	template<typename S>
	explicit Vec2_(const Vec2_<S>& src)
		:
		x( (T)src.x ),
		y( (T)src.y )
	{}
	Vec2_& operator=(const Vec2_& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	Vec2_ operator*(T rhs) const
	{
		return Vec2_(x * rhs, y * rhs);
	}
	Vec2_& operator*=(T rhs)
	{
		return *this = *this * rhs;
	}
	template<typename S>
	Vec2_ operator*(const Vec2_<S>& rhs) const
	{
		return Vec2_(x * (T)rhs.x, y * (T)rhs.y);
	}
	template<typename S>
	Vec2_ operator/(const Vec2_<S>& rhs) const
	{
		return Vec2_(x / (T)rhs.x, y / (T)rhs.y);
	}
	template<typename S>
	Vec2_ operator+(const Vec2_<S>& rhs) const
	{
		return Vec2_(x + (T)rhs.x, y + (T)rhs.y);
	}
	template<typename S>
	Vec2_ operator-(const Vec2_<S>& rhs) const
	{
		return Vec2_(x - (T)rhs.x, y - (T)rhs.y);
	}
	Vec2_& operator+=(const Vec2_& rhs)
	{
		return *this = *this + rhs;
	}
	Vec2_& operator-=(const Vec2_& rhs)
	{
		return *this = *this - rhs;
	}
	float GetLength() const
	{
		return sqrtf((float) GetLengthSq());
	}
	T GetLengthSq() const
	{
		return x * x + y * y;
	}
	void Normalized()
	{
		*this = GetNormalized();
	}
	Vec2_ GetNormalized() const
	{
		const float len = GetLength();
		if (len != 0.0f)
		{
			return *this * T(1.0f / len);
		}
		return *this;
	}
	operator Vec2_<T>() const
	{
		return Vec2_<T>((T)x, (T)y);
	}
public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2F;
typedef Vec2_<int> Vec2I;