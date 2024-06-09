#pragma once

#include "InputStreamService.h"
#include <map>

class KeyboardStreamService :
	public InputStreamService
{
private:
	const float MAX_FLUSH_EVENTS_TIMESECS = 1.f / 1000.f;
	
	std::map<int, bool> keysMapped;
	
	bool latestKeyPressed;
	bool isAnyKeyPressed;

	float flushEventsTime;

	void flush();

	void _processService(double tick) override;
	void _input(SDL_Event& e) override;


public:
	bool getKeyPressed(Uint32 keyCode);
	bool isKeyPressed();
};

