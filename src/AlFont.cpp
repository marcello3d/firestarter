#include "Audio.h"

using namespace std;

AlFont::AlFont() : data(NULL)
{
}


AlFont::~AlFont()
{
	if (data)
		destroy_sample(data);
}

bool AlFont::load(string fn)
{
	return (data = alfont_load_font(fn.c_str()));
}

