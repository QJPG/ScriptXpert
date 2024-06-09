#include "KeyboardStreamService.h"

void KeyboardStreamService::flush()
{
	if (!isAnyKeyPressed) {
		latestKeyPressed = -1;
	}
}

void KeyboardStreamService::_processService(double tick)
{
	//if (isAnyKeyPressed == false && latestKeyPressed == SDLK_SPACE)
	//	std::cout << "Space:" << "True" << std::endl;

	if (flushEventsTime < MAX_FLUSH_EVENTS_TIMESECS) {
		flushEventsTime += tick;
	}
	else {
		flush();
		flushEventsTime = 0;
	}
}

void KeyboardStreamService::_input(SDL_Event& e)
{
	switch (e.type) {
		case SDL_KEYDOWN:
			isAnyKeyPressed = true;
			latestKeyPressed = e.key.keysym.sym;

			keysMapped[e.key.keysym.sym] = true;
			break;

		case SDL_KEYUP:
			isAnyKeyPressed = false;

			keysMapped[e.key.keysym.sym] = false;
			break;
	}
}

bool KeyboardStreamService::getKeyPressed(Uint32 keyCode)
{
	return keysMapped[keyCode];
}

bool KeyboardStreamService::isKeyPressed()
{
	return isAnyKeyPressed;
}
