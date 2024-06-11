#include "DeviceTarget.h"

//std::map<int, bool> DeviceTarget::KEYSMAPPED;

DeviceTarget::DeviceTarget()
{
	ptr_DT_informer = new DT_Informer;
	ptr_DT_informer->DT_WSx = 1024;
	ptr_DT_informer->DT_WSy = 600;
	ptr_DT_informer->DT_Title = "Unnamed Runtime";
	ptr_DT_informer->DT_GLMAXv = 2;
	ptr_DT_informer->DT_GLMINv = 5;
	ptr_DT_informer->DT_FPS = 60;

	//window_SizeX = 1024;
	//window_SizeY = 600;
	//window_Title = "ScriptXpert Runtime";
	//window_OpenGLMaxVersion = 2;
	//window_OpenGLMinVersion = 5;
	//window_FramesPerSecond = 60;
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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ptr_DT_informer->DT_GLMAXv);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ptr_DT_informer->DT_GLMINv);

	ptr_SDL_window = SDL_CreateWindow(
		ptr_DT_informer->DT_Title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		ptr_DT_informer->DT_WSx,
		ptr_DT_informer->DT_WSy,
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
	Uint32 tick_Last;
	Uint32 tick_Current;

	ptr_SDL_event = new SDL_Event;

	while (!ptr_DT_informer->DT_Abort) {
		tick_Last = SDL_GetTicks();

		while (SDL_PollEvent(ptr_SDL_event)) {
			switch (ptr_SDL_event->type) {
				case SDL_QUIT:
					ptr_DT_informer->DT_Abort = true;
					break;
			
				case SDL_KEYDOWN:
					ptr_DT_informer->DT_KMAP[ptr_SDL_event->key.keysym.sym] = true;
					break;

				case SDL_KEYUP:
					ptr_DT_informer->DT_KMAP[ptr_SDL_event->key.keysym.sym] = false;
					break;
			}

			_input_event();
		}

		_process_update();
		_process_draw();

		tick_Current = SDL_GetTicks();
		ptr_DT_informer->DT_DeltaTick = (tick_Current - tick_Last) / 1000.f; //Convert MS to S

		if ((1000 / ptr_DT_informer->DT_FPS) > (tick_Current - tick_Last))
			SDL_Delay((1000 / ptr_DT_informer->DT_FPS) - (tick_Current - tick_Last));
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

void DeviceTarget::_bindLuaGlobals()
{
	lua_pushlightuserdata(getLuaState(), ptr_DT_informer);
	lua_setglobal(getLuaState(), "idevice");

	lua_pushinteger(getLuaState(), SDL_KEYDOWN);
	lua_setglobal(getLuaState(), "_KEYDOWN_");

	lua_pushinteger(getLuaState(), SDL_KEYUP);
	lua_setglobal(getLuaState(), "_KEYUP_");

	lua_pushinteger(getLuaState(), SDL_MOUSEBUTTONDOWN);
	lua_setglobal(getLuaState(), "_MOUSEBTNDOWN_");

	lua_pushinteger(getLuaState(), SDL_MOUSEBUTTONUP);
	lua_setglobal(getLuaState(), "_MOUSEBTNUP_");

	lua_pushinteger(getLuaState(), SDL_MOUSEMOTION);
	lua_setglobal(getLuaState(), "_MOUSEMOTION_");

	lua_pushinteger(getLuaState(), SDL_MOUSEWHEEL);
	lua_setglobal(getLuaState(), "_MOUSEWHEEL_");

	lua_pushinteger(getLuaState(), SDL_TEXTEDITING);
	lua_setglobal(getLuaState(), "_TEXTEDITING_");

	lua_pushinteger(getLuaState(), SDL_TEXTINPUT);
	lua_setglobal(getLuaState(), "_TEXTINPUT_");

	lua_pushinteger(getLuaState(), SDL_KEYMAPCHANGED);
	lua_setglobal(getLuaState(), "_KMAPCHANGED_");

	lua_pushinteger(getLuaState(), SDL_TEXTEDITING_EXT);
	lua_setglobal(getLuaState(), "_TEXTEDITINGEXT_");

	lua_pushinteger(getLuaState(), KMOD_NONE);
	lua_setglobal(getLuaState(), "_KMODNONE_");

	lua_pushinteger(getLuaState(), KMOD_LSHIFT);
	lua_setglobal(getLuaState(), "_KMODLSHIFT_");

	lua_pushinteger(getLuaState(), KMOD_RSHIFT);
	lua_setglobal(getLuaState(), "_KMODRSHIFT_");

	lua_pushinteger(getLuaState(), KMOD_LCTRL);
	lua_setglobal(getLuaState(), "_KMODLCTRL_");

	lua_pushinteger(getLuaState(), KMOD_RCTRL);
	lua_setglobal(getLuaState(), "_KMODRCTRL_");

	lua_pushinteger(getLuaState(), KMOD_LALT);
	lua_setglobal(getLuaState(), "_KMODLALT_");

	lua_pushinteger(getLuaState(), KMOD_RALT);
	lua_setglobal(getLuaState(), "_KMODRALT_");

	lua_pushinteger(getLuaState(), KMOD_LGUI);
	lua_setglobal(getLuaState(), "_KMODLGUI_");

	lua_pushinteger(getLuaState(), KMOD_RGUI);
	lua_setglobal(getLuaState(), "_KMODRGUI_");

	lua_pushinteger(getLuaState(), KMOD_NUM);
	lua_setglobal(getLuaState(), "_KMODNUM_");

	lua_pushinteger(getLuaState(), KMOD_CAPS);
	lua_setglobal(getLuaState(), "_KMODCAPS_");

	lua_pushinteger(getLuaState(), KMOD_MODE);
	lua_setglobal(getLuaState(), "_KMODMODE_");

	lua_pushinteger(getLuaState(), KMOD_SCROLL);
	lua_setglobal(getLuaState(), "_KMODSCROLL_");

	lua_pushinteger(getLuaState(), KMOD_CTRL);
	lua_setglobal(getLuaState(), "_KMODCTRL_");

	lua_pushinteger(getLuaState(), KMOD_SHIFT);
	lua_setglobal(getLuaState(), "_KMODSHIFT_");

	lua_pushinteger(getLuaState(), KMOD_ALT);
	lua_setglobal(getLuaState(), "_KMODALT_");

	lua_pushinteger(getLuaState(), KMOD_GUI);
	lua_setglobal(getLuaState(), "_KMODGUI_");

	lua_pushinteger(getLuaState(), KMOD_ALT | KMOD_CTRL);
	lua_setglobal(getLuaState(), "_KMODALTGR_");
}

void DeviceTarget::_bindLuaFunctions()
{
	lua_register(getLuaState(), "d_get_event_type", L_EVENTGETTYPE);
	lua_register(getLuaState(), "d_get_event_key_code", L_EVENTGETKEYCODE);
	lua_register(getLuaState(), "d_get_event_key_scan_code", L_EVENTGETKEYSCANCODE);
	lua_register(getLuaState(), "d_get_event_key_mod", L_EVENTGETKEYMOD);
	lua_register(getLuaState(), "d_get_keymapped", L_GETKEYSMAPPED);
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
		lua_pushnumber(getLuaState(), ptr_DT_informer->DT_DeltaTick);
		
		if (lua_pcall(getLuaState(), 1, 0, 0) != LUA_OK) {
			std::cerr << lua_tostring(getLuaState(), -1);
			lua_pop(getLuaState(), 1);
		}
	}
}

void DeviceTarget::_process_draw()
{
	SDL_GL_SetSwapInterval(ptr_DT_informer->DT_VSync);

	glClearColor(
		ptr_DT_informer->DT_CLEAR.r,
		ptr_DT_informer->DT_CLEAR.g,
		ptr_DT_informer->DT_CLEAR.b,
		ptr_DT_informer->DT_CLEAR.a
	);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	callLuaFunction("draw");

	SDL_GL_SwapWindow(ptr_SDL_window);
}

void DeviceTarget::_exit()
{
	callLuaFunction("exited");
}

int DeviceTarget::L_EVENTGETTYPE(lua_State* L)
{
	SDL_Event* laddress = static_cast<SDL_Event*>(lua_touserdata(L, 1));

	if (!laddress) return 0;

	int evtype = luaL_checkinteger(L, 2);

	lua_pushboolean(L, laddress->type == evtype);

	return 1;
}

int DeviceTarget::L_EVENTGETKEYCODE(lua_State* L)
{
	SDL_Event* laddress = static_cast<SDL_Event*>(lua_touserdata(L, 1));

	if (!laddress) return 0;

	int evkeycode = luaL_checkinteger(L, 2);

	lua_pushboolean(L, laddress->key.keysym.sym == evkeycode);

	return 1;
}

int DeviceTarget::L_EVENTGETKEYSCANCODE(lua_State* L)
{
	SDL_Event* laddress = static_cast<SDL_Event*>(lua_touserdata(L, 1));

	if (!laddress) return 0;

	int evkeyscancode = luaL_checkinteger(L, 2);

	lua_pushboolean(L, laddress->key.keysym.scancode == evkeyscancode);

	return 1;
}

int DeviceTarget::L_EVENTGETKEYMOD(lua_State* L)
{
	SDL_Event* laddress = static_cast<SDL_Event*>(lua_touserdata(L, 1));

	if (!laddress) return 0;

	Uint16 evkeymod = luaL_checkinteger(L, 2);

	lua_pushboolean(L, (laddress->key.keysym.mod & evkeymod));

	return 1;
}

int DeviceTarget::L_GETKEYSMAPPED(lua_State* L)
{
	DeviceTarget::DT_Informer* dt = static_cast<DeviceTarget::DT_Informer*>(lua_touserdata(L, 1));

	if (!dt) return 0;

	int kcode = luaL_checkinteger(L, 2);

	lua_pushboolean(L, dt->DT_KMAP[kcode]);

	return 1;
}
