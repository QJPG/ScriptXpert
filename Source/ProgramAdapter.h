#pragma once

#include <iostream>
#include <functional>
#include "lua.hpp"

class ProgramAdapter
{
private:
	lua_State* LA;

public:
	virtual ~ProgramAdapter() = default;

	void setLuaState(lua_State* L);
	void callLuaFunction(const char* func_name, unsigned int argc = 0, unsigned int argr = 0);
	void bindLuaFunction(const char* func_name, int(*F)(lua_State* L));
	
	virtual void bindLuaGlobals() = 0;
	virtual void bindLuaFunctions() = 0;

	lua_State* getLuaState();
};

