#pragma once

#include "script/lua/LuaWrapper.h"

namespace RE {
		
	class Engine {
	public:
		Engine();
		virtual ~Engine();

		bool Init();

	public:
		lua_State* L = nullptr;
		kaguya::State* Lua = nullptr;
	};

}