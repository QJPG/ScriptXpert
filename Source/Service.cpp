#include "Service.h"

UUID Service::nextID = 0;


Service::Service()
{
	ID = nextID;
	Service::nextID++;
}

void Service::processService(double tick)
{
	if (active) _processService(tick);
}

UUID Service::getID()
{
	return ID;
}
