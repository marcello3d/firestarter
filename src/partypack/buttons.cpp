#include <allegro.h>

#include "buttons.h"

////////////////////////////////////////////////////////////////////////////////
KeyboardButton::KeyboardButton(int _k) : k(_k)
{
}

bool KeyboardButton::isDown()
{
	return key[k];
}

////////////////////////////////////////////////////////////////////////////////
JoystickButton::JoystickButton(int _pad, int _btn) : pad(_pad), btn(_btn)
{
}

bool JoystickButton::isDown()
{
	return joy[pad].button[btn].b;
}

////////////////////////////////////////////////////////////////////////////////
JoystickDirection::JoystickDirection(int _pad, int _stick, int _axis, bool _dir) : pad(_pad), stick(_stick), axis(_axis), dir(_dir)
{
}

bool JoystickDirection::isDown()
{
	if (dir)
		return joy[pad].stick[stick].axis[axis].d2;
	return joy[pad].stick[stick].axis[axis].d1;
}

