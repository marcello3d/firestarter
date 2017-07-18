////////////////////////////////////////////////////////////////////////////////
#ifndef __region2_h__
#define __region2_h__

////////////////////////////////////////////////////////////////////////////////
#include "vector2d.h"
#include "size2d.h"

////////////////////////////////////////////////////////////////////////////////
template <typename T> class region2
{
public:
	T x, y, w, h;

	//////////
	region2() : x(0), y(0), w(0), h(0) { }
	region2(T _x, T _y, T _w, T _h) : x(_x), y(_y), w(_w), h(_h) { }
	region2(vector2<T> p, size2<T> m) : x(p.x), y(p.y), w(m.w), h(m.h) { }
	
	//////////
	inline region2<T> operator + (vector2<T> v) { return region2<T>(x + v.x, y + v.y, w, h); }
	inline region2<T> operator - (vector2<T> v) { return region2<T>(x - v.x, y - v.y, w, h); }
	
	//////////
	inline bool contains(vector2<T> i) { return ((i.x >= x) && (i.y >= y) && (i.x < x + w) && (i.y < y + h)); }
	inline bool contains(region2<T> r) { return ((r.x >= x) && (r.x < x + w) && (r.x + r.w < x + w) && (r.y >= y) && (r.y < y + h) && (r.y + r.h < y + h)); }
	
	inline bool fitsInside(region2<T> r) { return r.contains(*this); }
	
	//////////
	inline region2<T> intersect(region2<T> r)
	{
		region2<T> e(*this);
		
		int leftx = x - r.x;
		int topy = y - r.y;
		
		if (leftx < 0)
		{
			e.x -= leftx;
			e.w += leftx;
		}
		if (topy < 0)
		{
			e.y -= topy;
			e.h += topy;
		}
		
		if (e.w < 0) e.w = 0;
		if (e.h < 0) e.h = 0;
			
		return e;
	}
};
////////////////////////////////////////////////////////////////////////////////
typedef region2<int> region2i;
typedef region2<float> region2f;
typedef region2<double> region2d;

////////////////////////////////////////////////////////////////////////////////
#endif
