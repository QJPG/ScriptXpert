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

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) return APP_ERROR;

	//services init

	scriptServer = new ScriptServer;

	//script init

	if (scriptServer->initialize("x64\\Debug\\script.lua") != SCRIPT_SUCESS) {
		std::string allLogs = "Script Server Loggings:\n";

		for (unsigned int i = 0; i < scriptServer->logs.size(); i++) {
			allLogs += scriptServer->logs[i];
			allLogs += "\n";
		}

		showAlert(allLogs.c_str());

		return APP_ERROR;
	}

	return APP_SUCCESS;
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

		process_step(); //Step run and Draw Calls

		tick1 = SDL_GetTicks();
		window_deltaTime = (tick1 - tick0) / 1000.f; //Convert MS to S

		if ((1000 / window_FPS) > tick1 - tick0)
			SDL_Delay((1000 / window_FPS) - (tick1 - tick0));
	}
}

void AppDevice::process_input()
{
	//for (std::unique_ptr<Service>& ptr : services) {
	//	ptr.get()->event(sdl_event);
	//}

	InputStreamServiceManager::ListenEvents(sdl_event);
}

void AppDevice::process_step()
{
	//for (std::unique_ptr<Service>& ptr : services) {
	//	ptr.get()->step(window_deltaTime);
	//}

	InputStreamServiceManager::ProcessAllInputServices(window_deltaTime);
	StreamServiceManager::ProcessAllServices(window_deltaTime);

	scriptServer->callFunction("update");
}

void AppDevice::showAlert(const char* message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ScriptXpert Alert!", message, sdl_window);
}

