#include "AppDevice.h"

AppDevice::AppDevice(int _argc, const char * _argv[])
{
	window_sizex = 1024;
	window_sizey = 600;
	
	window_title = "ScriptXpert Runtime";

	window_OpenGLMaxVersion = 2;
	window_OpenGLMinVersion = 5;

	window_FPS = 60;
}

AppDevice::~AppDevice()
{
	SDL_GL_DeleteContext(sdl_gl_context);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}

int AppDevice::run()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return APP_ERROR;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, window_OpenGLMaxVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, window_OpenGLMinVersion);

	sdl_window = SDL_CreateWindow(
		window_title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_sizex,
		window_sizey,
		SDL_WINDOW_OPENGL
	);

	sdl_gl_context = SDL_GL_CreateContext(sdl_window);

	if (sdl_gl_context == NULL) return APP_ERROR;

	return APP_SUCCESS;

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) return APP_ERROR;
}

void AppDevice::step()
{
	bool stopStep = false;
	
	Uint32 tick0;
	Uint32 tick1;

	while (!stopStep) {
		tick0 = SDL_GetTicks();

		while (SDL_PollEvent(&sdl_event)) {
			if (sdl_event.type == SDL_QUIT) stopStep = true;

			process_input(); //IO
		}

		process_draw(); //Draw calls
		process_step(); //Step run

		tick1 = SDL_GetTicks();
		window_deltaTime = (tick1 - tick0) / 1000.f; //Convert MS to S

		if ((1000 / window_FPS) > tick1 - tick0)
			SDL_Delay((1000 / window_FPS) - (tick1 - tick0));
	}
}

void AppDevice::process_input()
{
}

void AppDevice::process_draw()
{
}

void AppDevice::process_step()
{
}
