#pragma once

#include <iostream>
#include <functional>
#include <map>
#include "lua.hpp"

class ProgramAdapter
{
private:
	lua_State* LA;

	virtual void _bindLuaGlobals() = 0;
	virtual void _bindLuaFunctions() = 0;

public:
	virtual ~ProgramAdapter() = default;

	void setLuaState(lua_State* L);
	void callLuaFunction(const char* func_name, unsigned int argc = 0, unsigned int argr = 0);
	void bindLuaFunction(const char* func_name, int(*F)(lua_State* L));

	lua_State* getLuaState();
};

