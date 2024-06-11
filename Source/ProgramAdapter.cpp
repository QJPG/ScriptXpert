#include "ProgramAdapter.h"

void ProgramAdapter::setLuaState(lua_State* L)
{
	LA = L;

	_bindLuaGlobals();
	_bindLuaFunctions();
}

void ProgramAdapter::callLuaFunction(const char* func_name, unsigned int argc, unsigned int argr)
{
	/*
	* EXAMPLE
	* ARGC > 0
		lua_pushnumber(getLuaState(), DeltaTime);
		callLuaFunction("update", 1);
	* ELSE
		callLuanFunction("update");
	*/


	if (!LA) return;
	
	lua_getglobal(LA, func_name);

	if (lua_isfunction(LA, -1)) {
		if (lua_pcall(LA, argc, argr, 0) != LUA_OK) {
			std::cerr << "ScriptXpert > LUA ERROR: " << lua_tostring(LA, -1) << std::endl;
			lua_pop(LA, 1);
		}
	}
	else {
		lua_pop(LA, 1);
	}	
}

void ProgramAdapter::bindLuaFunction(const char* func_name, int(*F)(lua_State* L))
{
	if (!LA) return;

	lua_register(LA, func_name, F);
}

lua_State* ProgramAdapter::getLuaState()
{
	return LA;
}
