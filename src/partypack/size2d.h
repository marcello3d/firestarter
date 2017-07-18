////////////////////////////////////////////////////////////////////////////////
#ifndef __size2_h__
#define __size2_h__

////////////////////////////////////////////////////////////////////////////////
template <typename T> class size2
{
public:
	T w, h;

	//////////
	size2() : w(0), h(0) { }
	size2(T _w, T _h) : w(_w), h(_h) { }

	//////////
	inline size2<T> operator * (T f) const
	{
		return size2<T>(w * f, h * f);
	}
	inline size2<T> operator *= (T f)
	{
		w *= f;
		h *= f;
		return *this;
	}
	//////////
	inline size2<T> operator / (T f) const
	{
		return size2<T>(w / f, h / f);
	}
	inline size2<T> operator /= (T f)
	{
		w /= f;
		h /= f;
		return *this;
	}

};
////////////////////////////////////////////////////////////////////////////////
typedef size2<int> size2i;
typedef size2<double> size2d;

////////////////////////////////////////////////////////////////////////////////
#endif
