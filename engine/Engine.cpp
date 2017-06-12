#include "Engine.h"
#include "lua.hpp"

RE::Engine::Engine()
{
	Init();
}

RE::Engine::~Engine()
{

}

bool RE::Engine::Init()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	return true;
}
