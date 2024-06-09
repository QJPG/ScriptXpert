#pragma once

#include "lua.hpp"
#include "Service.h"

class StreamService
{
public:
	static void BindScriptFunctions(lua_State* L);
};

