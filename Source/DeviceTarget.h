#pragma once

#include <vector>
#include <memory>
#include <map>

#include <SDL.h>
#undef main

#include <glad/glad.h>

#include "ProgramAdapter.h"

#define CSTR const char *
#define SUCCESS 0
#define ERROR 1
#define MSGERROR SDL_MESSAGEBOX_ERROR
#define MSGWARNING SDL_MESSAGEBOX_WARNING

class DeviceTarget : public ProgramAdapter {
public:
	/*
	DeviceTarget(int _argc, const char* _argv[]);
	~DeviceTarget();

	int run();
	void step();
	void process_input();
	void process_step();
	void showAlert(const char* message);
	*/
	
	DeviceTarget();
	~DeviceTarget();

	int initDevice();

	void run();
	void showOSMessage(Uint32 MSGSTATUS, CSTR message, CSTR title = "ScriptXpert");

private:
	/*
	Uint32 window_sizex;
	Uint32 window_sizey;
	Uint32 window_FPS;
	Uint32 window_OpenGLMaxVersion;
	Uint32 window_OpenGLMinVersion;

	const char * window_title;

	double window_deltaTime;

	SDL_Window* sdl_window = NULL;
	SDL_Event sdl_event;
	SDL_GLContext sdl_gl_context;

	ScriptServer* scriptServer;

	void bindScriptFunctions();

	static int _L_GETDELTATIME(lua_State* L);
	*/

	struct DT_Informer {
		uint32_t DT_WSx;
		uint32_t DT_WSy;
		uint32_t DT_FPS;
		uint32_t DT_GLMAXv;
		uint32_t DT_GLMINv;
		CSTR DT_Title;
		double DT_DeltaTick;
		std::map<int, bool> DT_KMAP;
		bool DT_Abort = false;
		SDL_Color DT_CLEAR {0, 0, 0, 255};
		int DT_VSync = -1;
	};

	DT_Informer* ptr_DT_informer;

	//uint32_t window_SizeX;
	//uint32_t window_SizeY;
	//uint32_t window_FramesPerSecond;
	//uint32_t window_OpenGLMaxVersion;
	//uint32_t window_OpenGLMinVersion;
	//CSTR window_Title;
	//double window_DeltaTime = 0.f;
	
	SDL_Window* ptr_SDL_window = NULL;
	SDL_Event* ptr_SDL_event;
	SDL_GLContext SDL_GL_context = 0;
	
	//static std::map<int, bool> KEYSMAPPED;

	void _start();
	void _input_event();
	void _process_update();
	void _process_draw();
	void _exit();

	void _bindLuaGlobals() override;
	void _bindLuaFunctions() override;

	static int L_EVENTGETTYPE(lua_State* L);
	static int L_EVENTGETKEYCODE(lua_State* L);
	static int L_EVENTGETKEYSCANCODE(lua_State* L); /*https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf -> page 0x07*/
	static int L_EVENTGETKEYMOD(lua_State* L);
	
	static int L_GETKEYSMAPPED(lua_State* L);

};