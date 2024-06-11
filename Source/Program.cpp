#include "Program.h"

Program::Program()
{
	device = new DeviceTarget();
	graphics = new GraphicsAdapter();
}

Program::~Program()
{
	delete graphics;
	delete device;

	lua_close(LState);
}

void Program::setup(int argc, const char* argv[])
{
	const char* script_filename = "";

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "S") == 0) script_filename = argv[i + 1];
	}

	LState = luaL_newstate();

	if (!LState) return;

	luaL_openlibs(LState);

	bindNativeGlobals();
	bindNativeFunctions();

	device->setLuaState(LState);
	graphics->setLuaState(LState);

	if (luaL_dofile(LState, script_filename) != LUA_OK) {
		std::cerr << lua_tostring(LState, -1) << std::endl;

		lua_pop(LState, 1);
	}

	device->initDevice();
	device->run();
}

void Program::bindNativeGlobals()
{
	//device->bindLuaGlobals();
}

void Program::bindNativeFunctions()
{
	lua_register(LState, "free", L_FREEPTR);
	lua_register(LState, "freeadapter", L_FREEADAPTER);

	//device->bindLuaFunctions();
}

void Program::LUAPUSHMEMADDR(lua_State* L, uintptr_t& ptr_addr)
{
	lua_pushinteger(L, static_cast<lua_Integer>(ptr_addr));
}

/*int Program::L_INITDEVICE(lua_State* L)
{
	DeviceTarget* device = new DeviceTarget;
	device->setLuaState(L);

	uintptr_t address = reinterpret_cast<uintptr_t>(device);

	LUAPUSHMEMADDR(L, address);

	return 1;
}*/

int Program::L_FREEPTR(lua_State* L)
{
	delete lua_touserdata(L, 1);

	return 0;
}

int Program::L_FREEADAPTER(lua_State* L)
{
	uintptr_t address = static_cast<uintptr_t>(luaL_checkinteger(L, 1));
		
	delete reinterpret_cast<ProgramAdapter*>(address);

	return 0;
}
