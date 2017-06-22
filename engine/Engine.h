#pragma once

#include "script/lua/LuaWrapper.h"
#include "base/Time.h"

namespace RE {
		
	class Engine {
	public:
		static Engine instance;

	public:
		Engine();
		virtual ~Engine();

		bool Init();
		void Update(float dt);
		void Render();
		void Loop();

		void SetFPS(int fps);

	protected:
		void initLuaEnginie();

	public:
		lua_State* L = nullptr;
		kaguya::State Lua;
		Time time;

	protected:
		double _interval = 1 / 60.0f;
		UI32 _fps = 60;
		class GameObject* _root = nullptr;
	};

}