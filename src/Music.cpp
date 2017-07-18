#include <aldumb.h>
#include "Music.h"

using namespace std;

Music::Music() : data(NULL)
{
}


Music::~Music()
{
	if (data)
		unload_duh(data);
}

bool Music::load(string fn)
{
	if (fn.substr(fn.size()-3)==".it")
		return (data = dumb_load_it(fn.c_str()));
	else if (fn.substr(fn.size()-3)==".xm")
		return (data = dumb_load_xm(fn.c_str()));
	return false;
}

