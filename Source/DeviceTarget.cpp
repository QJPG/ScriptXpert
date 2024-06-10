#include "DeviceTarget.h"

DeviceTarget::DeviceTarget()
{
	window_SizeX = 1024;
	window_SizeY = 600;
	window_Title = "ScriptXpert Runtime";
	window_OpenGLMaxVersion = 2;
	window_OpenGLMinVersion = 5;
	window_FramesPerSecond = 60;
}

DeviceTarget::~DeviceTarget()
{
	_exit();
	
	delete ptr_SDL_event;

	SDL_GL_DeleteContext(SDL_GL_context);
	SDL_DestroyWindow(ptr_SDL_window);
	SDL_Quit();
}

int DeviceTarget::initDevice()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return ERROR;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, window_OpenGLMaxVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, window_OpenGLMinVersion);

	ptr_SDL_window = SDL_CreateWindow(
		window_Title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_SizeX,
		window_SizeY,
		SDL_WINDOW_OPENGL
	);

	SDL_GL_context = SDL_GL_CreateContext(ptr_SDL_window);

	if (SDL_GL_context == NULL) return ERROR;
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) return ERROR;

	callLuaFunction("start");
	
	return SUCCESS;
}

void DeviceTarget::run()
{
	bool abort = false;

	Uint32 tick_Last;
	Uint32 tick_Current;

	ptr_SDL_event = new SDL_Event;

	while (!abort) {
		tick_Last = SDL_GetTicks();

		while (SDL_PollEvent(ptr_SDL_event)) {
			if (ptr_SDL_event->type == SDL_QUIT) abort = true;

			_input_event();
		}
		_process_update();
		_process_draw();

		tick_Current = SDL_GetTicks();
		window_DeltaTime = (tick_Current - tick_Last) / 1000.f; //Convert MS to S

		if ((1000 / window_FramesPerSecond) > (tick_Current - tick_Last))
			SDL_Delay((1000 / window_FramesPerSecond) - (tick_Current - tick_Last));
	}
}

void DeviceTarget::showOSMessage(Uint32 MSGSTATUS, CSTR message, CSTR title)
{
	SDL_ShowSimpleMessageBox(
		MSGSTATUS,
		title,
		message,
		ptr_SDL_window
	);
}

void DeviceTarget::bindLuaGlobals()
{
	lua_pushinteger(getLuaState(), SDL_KEYDOWN);
	lua_setglobal(getLuaState(), "KEYDOWN");

	lua_pushinteger(getLuaState(), SDL_KEYUP);
	lua_setglobal(getLuaState(), "KEYUP");

	lua_pushinteger(getLuaState(), SDL_MOUSEBUTTONDOWN);
	lua_setglobal(getLuaState(), "MOUSEBTNDOWN");

	lua_pushinteger(getLuaState(), SDL_MOUSEBUTTONUP);
	lua_setglobal(getLuaState(), "MOUSEBTNUP");

	lua_pushinteger(getLuaState(), SDL_MOUSEMOTION);
	lua_setglobal(getLuaState(), "MOUSEMOTION");
}

void DeviceTarget::bindLuaFunctions()
{
	lua_register(getLuaState(), "d_get_event_type", L_EVENTGETTYPE);
}

void DeviceTarget::_start()
{
	callLuaFunction("start");
}

void DeviceTarget::_input_event()
{
	lua_getglobal(getLuaState(), "event");
	if (lua_isfunction(getLuaState(), -1)) {
		
		lua_pushlightuserdata(getLuaState(), ptr_SDL_event);

		if (lua_pcall(getLuaState(), 1, 0, 0) != LUA_OK) {
			std::cerr << lua_tostring(getLuaState(), -1);
			lua_pop(getLuaState(), 1);
		}
	}
}

void DeviceTarget::_process_update()
{
	lua_getglobal(getLuaState(), "update");
	if (lua_isfunction(getLuaState(), -1)) {
		lua_pushnumber(getLuaState(), window_DeltaTime);
		
		if (lua_pcall(getLuaState(), 1, 0, 0) != LUA_OK) {
			std::cerr << lua_tostring(getLuaState(), -1);
			lua_pop(getLuaState(), 1);
		}
	}
}

void DeviceTarget::_process_draw()
{
	callLuaFunction("draw");
}

void DeviceTarget::_exit()
{
	callLuaFunction("exit");
}

int DeviceTarget::L_EVENTGETTYPE(lua_State* L)
{
	SDL_Event* laddress = static_cast<SDL_Event*>(lua_touserdata(L, 1));
	int evtype = luaL_checkinteger(L, 2);

	if (laddress->type == evtype) {
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}

	return 1;
}
