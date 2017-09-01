#include "LuaWrapper.h"
#include "engine/Engine.h"
#include "engine/component/Transform.h"
#include "engine/object/GameObject.h"
#include "base/String.h"
#include "platform/FileUtils.h"
#include "platform/Platform.h"

extern "C"
{
	int luaLoader(lua_State *L) {
		static std::string filename;
		filename.assign(luaL_checkstring(L, -1));

		if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".lua") {
			filename = filename.substr(0, filename.size() - 4);
		}
		
		filename = RE::String::Replace(filename, ".", "/");

		filename.append(".lua");
		auto&& data = RE::FileUtils::getInstance()->getData(filename);
		if (!data.isNull()) {

			auto bytes = (unsigned char*)data.getBytes();
			unsigned long size = data.getSize();

			if (size >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF) { // UTF-8 with BOM
				bytes += 3;
				size -= 3;
			}

			if (luaL_loadbuffer(L, (char*)bytes, size, filename.c_str()) != 0) {
				luaL_error(L, "error loading module %s from file %s :\n\t%s",
					lua_tostring(L, 1), filename.c_str(), lua_tostring(L, -1));
			}
			return 1;
		} else {
			bool isFileExists = RE::FileUtils::getInstance()->isFileExists(filename);
			Log("Lua File Not Found!!! [%s] [%s]", filename.c_str(), isFileExists ? "true" : "false");
			luaL_error(L, "error loading module %s from file %s :\n\t%s",
				lua_tostring(L, 1), filename.c_str(), lua_tostring(L, -1));

			luaL_dostring(L, "print(debug.traceback())");
		}
		return 0;
	}

	void addLuaLoader(lua_State *L, lua_CFunction func) {
		if (!func) return;

		lua_getglobal(L, "package");                                  /* L: package */
		lua_getfield(L, -1, "searchers");                               /* L: package, loaders */
																			// insert loader into index 2
		lua_pushcclosure(L, func, 0);                                   /* L: package, loaders, func */
		for (int i = (int)lua_rawlen(L, -2) + 1; i > 2; --i) {
			lua_rawgeti(L, -2, i - 1);                                /* L: package, loaders, func, function */
																			// we call lua_rawgeti, so the loader table now is at -3
			lua_rawseti(L, -3, i);                                    /* L: package, loaders, func */
		}
		lua_rawseti(L, -2, 2);                                        /* L: package, loaders */
																			// set loaders into package
		lua_setfield(L, -2, "searchers");                               /* L: package */

		lua_pop(L, 1);
	}
}

void RE::LuaWrapper::registerLua() {
	addLuaLoader(Engine::instance.L, luaLoader);


	Engine::instance.Lua["Transform"].setClass(kaguya::UserdataMetatable<Transform>()
		.addProperty("x", &Transform::x)
		.addProperty("y", &Transform::y)
		.addProperty("sx", &Transform::sx)
		.addProperty("sy", &Transform::sy)
		.addProperty("ax", &Transform::ax)
		.addProperty("ay", &Transform::ay)
		.addProperty("w", &Transform::w)
		.addProperty("h", &Transform::h)
		.addProperty("rotation", &Transform::rotation)
	);

	Engine::instance.Lua["GameObject"].setClass(kaguya::UserdataMetatable<GameObject>()
		.setConstructors<GameObject(), GameObject(const char*)>()
		.addProperty("name", &GameObject::name)
		.addProperty("transform", &GameObject::$transform)
		.addFunction("AddComponent", &GameObject::AddScriptComponent)
		.addFunction("RemoveComponent", &GameObject::RemoveScriptComponent)
	);

	Engine::instance.Lua["Engine"].setClass(kaguya::UserdataMetatable<Engine>()
		.addProperty("root", &Engine::root)
	);

	Engine::instance.Lua["Engine"]["instance"] = &Engine::instance;

}
