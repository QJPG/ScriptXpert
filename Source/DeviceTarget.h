#pragma once

#include <vector>
#include <memory>
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
	void bindLuaGlobals() override;
	void bindLuaFunctions() override;

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

	uint32_t window_SizeX;
	uint32_t window_SizeY;
	uint32_t window_FramesPerSecond;
	uint32_t window_OpenGLMaxVersion;
	uint32_t window_OpenGLMinVersion;
	CSTR window_Title;
	double window_DeltaTime = 0.f;
	SDL_Window* ptr_SDL_window = NULL;
	SDL_Event* ptr_SDL_event;
	SDL_GLContext SDL_GL_context = 0;

	void _start();
	void _input_event();
	void _process_update();
	void _process_draw();
	void _exit();

	static int L_EVENTGETTYPE(lua_State* L);

};