#include "ScriptServer.h"

ScriptServer::ScriptServer()
{
	lua_LState = luaL_newstate();

	if (!lua_LState) {
		logs.push_back("Create Lua state error!");

		std::cerr << "Error on create Lua state!" << std::endl;
	}

	luaL_openlibs(lua_LState);
	
	InputStreamServiceManager::BindScriptFunctions(lua_LState);
}

ScriptServer::~ScriptServer()
{
	lua_close(lua_LState);
}

int ScriptServer::initialize(const char * script)
{
	//std::cerr << std::filesystem::current_path() / script;

	std::ifstream scriptFile( script );

	if (scriptFile.is_open() == false) {
		//std::cerr << "Error on open script file!" << std::endl;

		logs.push_back("Error on open script file!");

		return SCRIPT_ERROR;
	}

	std::string source;
	//std::streampos len;

	//len = scriptFile.tellg();
	//source = new char[len];

	//scriptFile.seekg(0, std::ios::beg);
	//scriptFile.read(source, len);

	//Wtf is this?
	source = std::string((std::istreambuf_iterator<char>(scriptFile)), std::istreambuf_iterator<char>());

	scriptFile.close();

	bool scriptExecOK = true;
	
	if (luaL_dostring(lua_LState, source.c_str()) != LUA_OK) {
		scriptExecOK = false;
		
		//std::cerr << "\nScript(lua_dostring) error: " << lua_tostring(lua_LState, -1) << std::endl;
		logs.push_back(lua_tostring(lua_LState, -1));

		lua_pop(lua_LState, 1);
	}

	//delete [] source;

	callFunction("start");

	if (scriptExecOK)
		return SCRIPT_SUCESS;
	else
		return SCRIPT_ERROR;
}

void ScriptServer::bindFunction(const char* funcName, lua_CFunction callback)
{
	if (lua_LState) lua_register(lua_LState, funcName, callback);
}

int ScriptServer::callFunction(const char* funcName)
{
	bool returnStatus = SCRIPT_SUCESS;

	if (lua_LState) {
		lua_getglobal(lua_LState, funcName);

		if (lua_isfunction(lua_LState, -1)) {
			int callres = lua_pcall(lua_LState, 0, 0, 0);

			if (callres != LUA_OK) {
				logs.push_back(lua_tostring(lua_LState, -1));
				returnStatus = SCRIPT_ERROR;
				lua_pop(lua_LState, 1);
			}
		}
		else {
			std::string errmsg = "Function not found:";
			errmsg += funcName;

			logs.push_back(errmsg.c_str());
			returnStatus = SCRIPT_ERROR;
			lua_pop(lua_LState, 1);
		}
	}

	return returnStatus;
}