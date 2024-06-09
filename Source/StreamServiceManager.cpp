#include "StreamServiceManager.h"

std::vector<std::unique_ptr<Service>> StreamServiceManager::services;

void StreamServiceManager::AddService(std::unique_ptr<Service> uptr)
{
	services.push_back(std::move(uptr));
}

void StreamServiceManager::RemoveService(UUID ID)
{
	auto item = std::find_if(
		services.begin(),
		services.end(),
		[ID](const std::unique_ptr<Service>& lptr) {
			return lptr.get()->getID() == ID;
		});

	if (item != services.end()) services.erase(item);
}

void StreamServiceManager::ProcessAllServices(double tick)
{
	for (std::unique_ptr<Service>& lptr : services) {
		lptr.get()->processService(tick);
	}
}

Service* StreamServiceManager::GetService(UUID ID)
{
	for (std::unique_ptr<Service>& lptr : services) {
		if (lptr.get()->getID() == ID) return lptr.get();
	}

	return nullptr;
}

void StreamServiceManager::BindScriptFunctions(lua_State* L)
{
	StreamService::BindScriptFunctions(L);
}
