#include "Engine.h"

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

	Lua = new kaguya::State(L);

	return true;
}
