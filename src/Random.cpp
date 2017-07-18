#include <ctime>
#include <cmath>
#include <cstdlib>
#include "Random.h"

namespace Random {

	static _INT64 seed = time(0);
	static bool haveNextNextGaussian = false;
	static double nextNextGaussian;

	//copied from http://java.sun.com/j2se/1.4.2/docs/api/java/util/Random.html
	double gaussian() {
		if (haveNextNextGaussian) {
			haveNextNextGaussian = false;
			return nextNextGaussian;
		}
		else {
			double v1, v2, s;
			do {
				v1 = 2 * doubleIn1() - 1;   // between -1.0 and 1.0
				v2 = 2 * doubleIn1() - 1;   // between -1.0 and 1.0
				s = v1 * v1 + v2 * v2;
				} while (s >= 1 || s == 0);
			double multiplier = sqrt(-2 * log(s)/s);
			nextNextGaussian = v2 * multiplier;
			haveNextNextGaussian = true;
			return v1 * multiplier;
		}
	}

	//copied from http://java.sun.com/j2se/1.4.2/docs/api/java/util/Random.html
	void setSeed(_INT64 nseed) {
		seed = (nseed ^ 0x5DEECE66DLL) & ((1LL << 48) - 1);
	}

	//copied from http://java.sun.com/j2se/1.4.2/docs/api/java/util/Random.html
	int next(int bits) {
		seed = (seed * 0x5DEECE66DLL + 0xBLL) & ((1LL << 48) - 1);
		return (int)(seed >> (48 - bits));
	}

	//copied from http://java.sun.com/j2se/1.4.2/docs/api/java/util/Random.html
	double doubleIn1() {
		return (((_INT64)next(26) << 27) + next(27))
				/ (double)(1LL << 53);
	}

	//copied from http://java.sun.com/j2se/1.4.2/docs/api/java/util/Random.html
	float floatIn1() {
		return next(24) / ((float)(1 << 24));
	}

	//copied from http://java.sun.com/j2se/1.4.2/docs/api/java/util/Random.html
	int intIn(int max) {
		if (max<=0)
            max=(-max);
		if ((max & -max) == max)  // i.e., n is a power of 2
			return (int)((max * (_INT64)next(31)) >> 31);

		int bits, val;
		do {
			bits = next(31);
			val = bits % max;
		} while(bits - val + (max-1) < 0);
		return val;
	}

	int intIn1() {
		return next(1);
	}

	int intInRange(int min, int max) {
		return intIn(max-min)+min;
	}


}; // end namespace
