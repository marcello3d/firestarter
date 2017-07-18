#ifndef _AL_FONT_H
#define _AL_FONT_H

#include <alfont.h>
#include <string>

#include "partypack/resource.h"

class AlFont : public Resource
{
protected:
	ALFONT_FONT *data;
public:

	AlFont();
	~AlFont();

	bool load(std::string fn);

	inline ALFONT_FONT *get() { return data; }

};

#endif
