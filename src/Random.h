#ifndef _RANDOM_H
#define _RANDOM_H

#include <cstdlib>

#define _INT64	long long

namespace Random {
	double gaussian();
	void setSeed(_INT64 nSeed);
	int next(int bits);
	float floatIn1();
	double doubleIn1();
	int intIn(int max);
	int intIn1();
	int intInRange(int min, int max);

}; // end namespace


#endif
