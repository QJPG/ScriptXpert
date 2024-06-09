#pragma once

#include "StreamService.h"

class StreamServiceManager : public StreamService
{
private:
	static std::vector<std::unique_ptr<Service>> services;

public:
	//virtual ~StreamServiceManager() = default;

	static void AddService(std::unique_ptr<Service> uptr);
	static void RemoveService(UUID ID);
	static void ProcessAllServices(double tick);

	static Service* GetService(UUID ID);

	static void BindScriptFunctions(lua_State* L);

};

