#include "InputStreamServiceManager.h"

std::vector<std::unique_ptr<InputStreamService>> InputStreamServiceManager::inputStreamServices;

void InputStreamServiceManager::AddInputService(std::unique_ptr<InputStreamService> uptr)
{
	inputStreamServices.push_back(std::move(uptr));
}

void InputStreamServiceManager::RemoveInputService(UUID ID)
{
	auto item = std::find_if(
		inputStreamServices.begin(),
		inputStreamServices.end(),
		[ID](const std::unique_ptr<InputStreamService>& lptr) {
			return lptr.get()->getID() == ID;
		});

	if (item != inputStreamServices.end()) inputStreamServices.erase(item);
}

void InputStreamServiceManager::ListenEvents(SDL_Event& E)
{
	for (std::unique_ptr<InputStreamService>& lptr : inputStreamServices) {
		lptr.get()->input(E);
	}
}

void InputStreamServiceManager::ProcessAllInputServices(double tick)
{
	for (std::unique_ptr<InputStreamService>& lptr : inputStreamServices) {
		lptr.get()->processService(tick);
	}
}

void InputStreamServiceManager::BindScriptFunctions(lua_State* L)
{
	StreamService::BindScriptFunctions(L);

	lua_register(L, "InputService", _L_INPUTSERVICE);
	lua_register(L, "GetKeyPressed", _L_GETKEYPRESSED);
}

InputStreamService* InputStreamServiceManager::GetInputStreamService(UUID ID)
{
	for (std::unique_ptr<InputStreamService>& lptr : inputStreamServices) {
		if (lptr.get()->getID() == ID) return lptr.get();
	}

	return nullptr;
}

int InputStreamServiceManager::_L_INPUTSERVICE(lua_State* L)
{
	const char* serviceName = luaL_checkstring(L, 1);

	std::unique_ptr<InputStreamService> what;

	if (std::strcmp(serviceName, "keyboard") == 0)
		what = std::make_unique<KeyboardStreamService>();
	else
		std::cerr << "InputStreamService(" << serviceName << ") has not exists!" << std::endl;

	if (what.get() != nullptr) {
		lua_pushinteger(L, what.get()->getID());
		AddInputService(std::move(what));

		return 1;
	}

	lua_pushinteger(L, -1);

	return 1;
}

int InputStreamServiceManager::_L_GETKEYPRESSED(lua_State* L)
{
	int streamID = luaL_checkinteger(L, 1);

	if (streamID < inputStreamServices.size() - 1) {
		lua_pushboolean(L, false);
		
		return 1;
	}

	InputStreamService* stream = GetInputStreamService(streamID);
	const char* keyname = luaL_checkstring(L, 2);

	if (stream != nullptr) {
		KeyboardStreamService* out = dynamic_cast<KeyboardStreamService*>(stream);
		
		if (out != nullptr) {
			lua_pushboolean(L, out->getKeyPressed(SDL_GetKeyFromName(keyname)));

			return 1;
		}

		delete out;
	}

	lua_pushboolean(L, false);

	return 1;
}
