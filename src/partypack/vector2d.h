////////////////////////////////////////////////////////////////////////////////
#ifndef __vector2_h__
#define __vector2_h__

////////////////////////////////////////////////////////////////////////////////
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
template <typename T> class vector2
{
public:
	T x, y;

	//////////
	vector2() : x(0), y(0) { }
	vector2(T _x, T _y) : x(_x), y(_y) { }
	template <typename S> vector2(const vector2<S> &v) : x((T)v.x), y((T)v.y) { }
	
	//////////
	inline vector2<T> operator + (const vector2<T> v) const
	{
		return vector2<T>(x + v.x, y + v.y);
	}
	inline vector2<T> operator += (const vector2<T> v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	//////////
	inline vector2<T> operator - (const vector2<T> v) const
	{
		return vector2<T>(x - v.x, y - v.y);
	}
	inline vector2<T> operator -= (const vector2<T> v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	inline vector2<T> operator - () const
	{
		return vector2<T>(-x, -y);
	}
	
	//////////
	inline vector2<T> operator * (T f) const
	{
		return vector2<T>(x * f, y * f);
	}
	inline vector2<T> operator *= (T f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	//////////
	inline vector2<T> operator / (T f) const
	{
		return vector2<T>(x / f, y / f);
	}
	inline vector2<T> operator /= (T f)
	{
		x /= f;
		y /= f;
		return *this;
	}
	
	//////////
	inline T dot(vector2<T> v) const
	{
		return x*v.x + y*v.y;
	}
	
	//////////
	inline void normalize()
	{
		double m = magnitude();
		x /= m;
		y /= m;
	}
	
	//////////
	inline void clamp(vector2<T> min, vector2<T> max)
	{
		x = (x < min.x ? min.x : (x > max.x ? max.x : x));
		y = (y < min.y ? min.y : (y > max.y ? max.y : y));
	}
	
	//////////
	inline double magnitude() const { return std::sqrt(x*x + y*y); }
};
////////////////////////////////////////////////////////////////////////////////
typedef vector2<float> vector2f;
typedef vector2<double> vector2d;
typedef vector2<int> vector2i;

////////////////////////////////////////////////////////////////////////////////
#endif
