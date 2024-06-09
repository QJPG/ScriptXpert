#include "InputStreamService.h"

void InputStreamService::input(SDL_Event& E)
{
	if (active) _input(E);
}
