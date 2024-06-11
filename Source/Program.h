#pragma once

#include <iostream>
#include <memory>

#include "DeviceTarget.h"
#include "GraphicsAdapter.h"

class Program
{
public:
	Program();
	~Program();

	void setup(int argc, const char * argv[]);

private:
	lua_State* LState;
	DeviceTarget* device;
	GraphicsAdapter* graphics;

	void bindNativeGlobals();
	void bindNativeFunctions();

	static void LUAPUSHMEMADDR(lua_State* L, uintptr_t& ptr_addr);
	
	static int L_FREEPTR(lua_State* L);
	static int L_FREEADAPTER(lua_State* L);
};

