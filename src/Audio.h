#ifndef _AUDIO_H
#define _AUDIO_H

#include <allegro.h>
#include <string>

#include "partypack/resource.h"

class Audio : public Resource
{
protected:
	SAMPLE *data;
public:

	Audio();
	~Audio();

	bool load(std::string fn);

	inline SAMPLE *getSAMPLE() { return data; }

};

#endif
