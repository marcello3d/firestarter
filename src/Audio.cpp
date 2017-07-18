#include "Audio.h"

using namespace std;

Audio::Audio() : data(NULL)
{
}


Audio::~Audio()
{
	if (data)
		destroy_sample(data);
}

bool Audio::load(string fn)
{
	return (data = load_sample(fn.c_str()));
}

