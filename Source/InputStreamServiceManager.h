#pragma once

#include "InputStreamService.h"
#include <vector>
#include "StreamService.h"
#include "KeyboardStreamService.h"


class InputStreamServiceManager : public StreamService
{
private:
	static std::vector<std::unique_ptr<InputStreamService>> inputStreamServices;

	static int _L_INPUTSERVICE(lua_State* L);
	static int _L_GETKEYPRESSED(lua_State* L);

public:
	//virtual ~InputStreamServiceManager() = default;
	
	static void AddInputService(std::unique_ptr<InputStreamService> uptr);
	static void RemoveInputService(UUID ID);
	static void ListenEvents(SDL_Event& E);
	static void ProcessAllInputServices(double tick);
	static void BindScriptFunctions(lua_State* L);

	static InputStreamService* GetInputStreamService(UUID ID);
};

