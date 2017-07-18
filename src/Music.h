#ifndef _MUSIC_H
#define _MUSIC_H

#include <aldumb.h>
#include <vector>
#include <string>

#include "partypack/resource.h"

class Music : public Resource
{
protected:
	DUH *data;
public:

	Music();
	~Music();

	bool load(std::string fn);

	inline DUH *getDUH() { return data; }

};

#endif
