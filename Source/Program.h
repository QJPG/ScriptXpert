#pragma once

#include <iostream>
#include <memory>

#include "DeviceTarget.h"

class Program
{
public:
	Program();
	~Program();

	void setup();

private:
	lua_State* LState;
	DeviceTarget* device;

	void bindNativeGlobals();
	void bindNativeFunctions();

	static void LUAPUSHMEMADDR(lua_State* L, uintptr_t& ptr_addr);
	static int L_FREEADAPTER(lua_State* L);
};

