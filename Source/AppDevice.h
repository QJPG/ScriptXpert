#pragma once

#include <vector>
#include <memory>
#include <SDL.h>
#undef main

#include <glad/glad.h>

#include "StreamServiceManager.h"
#include "InputStreamServiceManager.h"

#include "ScriptServer.h"

#define APP_SUCCESS 0
#define APP_ERROR 1

#define SERVICE_INPUTSTREAM "input"

class AppDevice {
public:
	AppDevice(int _argc, const char * _argv []);
	~AppDevice();

	int run();
	void step();
	void process_input();
	void process_step();
	void showAlert(const char* message);

private:
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
};