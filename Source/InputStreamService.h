#pragma once

/*
Abstract Class for all Input Stream Services!
Managed by InputStreamServiceManager
*/

#include "Service.h"

class InputStreamService:
	public Service
{
public:
	virtual ~InputStreamService() = default;

	void input(SDL_Event& E);

	//void input(SDL_Event& sdlEvent) override;
	//void run(double deltaTime) override;

private:
	virtual void _input(SDL_Event& E) = 0;
	//const float MAX_FLUSH_EVENTS_TIMESECS = 1.f / 1000.f;

	//bool isAnyKeyPressed;
	//std::map<int, bool> keysMapped;
	//int latestKeyPressed = -1;
	//float flushEventsTime;

	//void flush();
};

