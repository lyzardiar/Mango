#pragma once
#include "IComponent.h"
#include "script/lua/LuaWrapper.h"
#include "base/String.h"
#include "base/Path.h"

namespace RE {
	class ScriptComponent : public IComponent {
	public:
		ScriptComponent() {}
		ScriptComponent(Path path, class GameObject* go);
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
		
		kaguya::LuaTable Load(Path& path);
		void Clear();
	protected:
		void setupFuncs();
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

		bool isValid = true;
		Path path;
		StaticString<64> baseName;

		kaguya::LuaTable Class;
		kaguya::LuaFunction luaFuncs[FuncType::MAX_FUNCS];
	};

}