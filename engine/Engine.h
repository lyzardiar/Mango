#pragma once

struct lua_State;

namespace RE {
		
	class Engine {
	public:
		Engine();
		virtual ~Engine();

		bool Init();

	public:
		lua_State* L = nullptr;
	};

}