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
		bool Loop(float dt);

		void SetFPS(int fps);
		UI32 GetTextureHandle();

	protected:
		void initLuaEnginie();

	public:
		lua_State* L = nullptr;
		class GameObject* root = nullptr;

		kaguya::State Lua;
		Time time;
		class Camera& camera;
		class InputSystem& input;

	protected:
		double _interval = 1 / 60.0f;
		UI32 _fps = 60;

		bool _isInited = false;

		class FrameBuffer* _fbo = nullptr;
	};

}