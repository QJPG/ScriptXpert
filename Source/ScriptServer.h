#pragma once

#define SCRIPT_SUCESS 0
#define SCRIPT_ERROR 1

#include <iostream>
#include <fstream>
#include <filesystem>
#include "lua.hpp"

#include <vector>

#include "StreamServiceManager.h"
#include "InputStreamServiceManager.h"

class ScriptServer
{
public:
	//StreamServiceManager streamServiceManager;
	//InputStreamServiceManager inputStreamServiceManager;
	
	std::vector<const char*> logs;

	ScriptServer();
	~ScriptServer();

	int initialize(const char * script);
	void bindFunction(const char* funcName, lua_CFunction callback);
	int callFunction(const char* funcName);

private:
	lua_State* lua_LState;
};

