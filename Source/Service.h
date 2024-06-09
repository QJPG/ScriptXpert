#pragma once

#define UUID unsigned short

#include <iostream>
#include <vector>
#include <SDL_events.h>

class Service
{
public:
	bool active = true;

	Service();
	virtual ~Service() = default;
	//virtual void run(double _deltaTime) = 0;
	//virtual void input(SDL_Event& _sdl_ev) = 0;

	//void event(SDL_Event& sdl_ev);
	//void step(double deltaTime);

	void processService(double tick);
	UUID getID();

private:
	static UUID nextID;
	UUID ID;

	virtual void _processService(double _tick) = 0;
};

