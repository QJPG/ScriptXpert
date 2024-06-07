#pragma once

#include <SDL.h>
#undef main

#include <glad/glad.h>

#define APP_SUCCESS 0
#define APP_ERROR 1

class AppDevice {
public:
	AppDevice(int _argc, const char * _argv []);
	~AppDevice();

	int run();
	void step();
	void process_input();
	void process_draw();
	void process_step();

private:
	Uint32 window_sizex;
	Uint32 window_sizey;
	Uint32 window_FPS;
	Uint32 window_deltaTime;
	Uint32 window_OpenGLMaxVersion;
	Uint32 window_OpenGLMinVersion;

	const char * window_title;

	SDL_Window* sdl_window = NULL;
	SDL_Event sdl_event;
	SDL_GLContext sdl_gl_context;
};