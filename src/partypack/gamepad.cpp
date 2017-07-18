#include "gamepad.h"

Gamepad::Gamepad()
{
}
Gamepad::Gamepad(int numkeys)
{
	bindings.resize(numkeys);
	for (int i = 0; i < numkeys; ++i)
		bindings[i] = 0;
}

Gamepad::~Gamepad()
{
}

bool Gamepad::isDown(int id)
{
	if (id >= 0 && id < (int)bindings.size() && bindings[id])
		return bindings[id]->isDown();
	return false;
}

void Gamepad::bind(int id, Button *btn)
{
	if ((id < 0) || (id >= (int)bindings.size()))
		return;
	if (bindings[id])
		delete bindings[id];
	bindings[id] = btn;
}
