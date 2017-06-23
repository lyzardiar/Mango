#pragma once
#include "IComponent.h"
#include "script/lua/LuaWrapper.h"

namespace RE {
	class ScriptComponent : public IComponent {
	public:
		ScriptComponent() {}
		ScriptComponent(const char* path);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnDestroy() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		virtual const char* TypeName() override;
	public:
		std::string path;

		kaguya::LuaTable Class;
	};

}