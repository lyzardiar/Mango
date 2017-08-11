#pragma once
#include "IComponent.h"
#include "script/lua/LuaWrapper.h"

namespace RE {
	class ScriptComponent : public IComponent {
	public:
		ScriptComponent() {}
		ScriptComponent(const char* path, class GameObject* go);
		~ScriptComponent();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update(float dt) override;
		virtual void OnDestroy() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		virtual const char* TypeName() override;

		void Reload();
		virtual void OnGUI() override;
	public:
		enum FuncType {
			AWAKE = 0,
			START = 1,
			UPDATE = 2,
			ONENABLE = 3,
			ONDISABLE = 4,
			ONDESTROY = 5,

			MAX_FUNCS,
		};

		std::string path;

		kaguya::LuaTable Class;
		kaguya::LuaFunction luaFuncs[FuncType::MAX_FUNCS];
	};

}