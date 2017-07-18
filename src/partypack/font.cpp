////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <allegro.h>

#include "font.h"

////////////////////////////////////////////////////////////////////////////////
using namespace std;

////////////////////////////////////////////////////////////////////////////////
/*
* Default ctor
*/
Font::Font() : originaldata(0)
{
}

////////////////////////////////////////////////////////////////////////////////
/*
* Zum zum zum.
*/
Font::Font(string fn) : originaldata(0)
{
	load(fn);
}

////////////////////////////////////////////////////////////////////////////////
Font::~Font()
{
	if (originaldata)
		unload_datafile_object(originaldata);
}

////////////////////////////////////////////////////////////////////////////////
/*
* Load from file. Implements Resource.
*/
bool Font::load(string fn)
{
	// Already loaded?
	ASSERT(!converted);

	// Fix palette if necessary
	DATAFILE *palette = load_datafile_object(fn.c_str(), "palette");
	if (!palette)
		return false;
	select_palette((RGB *)palette->dat);
	unload_datafile_object(palette);
	
	// Get data
	originaldata = load_datafile_object(fn.c_str(), "data");
	if (!originaldata)
		return false;
	
	return true;
}

