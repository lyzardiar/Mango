#pragma once

#include "lua.hpp"
#include "core/base/Types.h"

namespace RE {
	namespace LuaWrapper {

		///////////// Value Begin
		template <typename T> inline T Value(lua_State* L, int index)
		{
			return (T)lua_touserdata(L, index);
		}
		template <> inline int Value(lua_State* L, int index)
		{
			return (int)lua_tointeger(L, index);
		}
		template <> inline I64 Value(lua_State* L, int index)
		{
			return (I64)lua_tointeger(L, index);
		}
		template <> inline UI32 Value(lua_State* L, int index)
		{
			return (UI32)lua_tointeger(L, index);
		}
		template <> inline UI64 Value(lua_State* L, int index)
		{
			return (UI64)lua_tointeger(L, index);
		}
		template <> inline bool Value(lua_State* L, int index)
		{
			return lua_toboolean(L, index) != 0;
		}
		template <> inline float Value(lua_State* L, int index)
		{
			return (float)lua_tonumber(L, index);
		}
		template <> inline const char* Value(lua_State* L, int index)
		{
			return lua_tostring(L, index);
		}
		template <> inline void* Value(lua_State* L, int index)
		{
			return lua_touserdata(L, index);
		}
		///////////// Value End

		///////////// ValueName Begin
		template <typename T> inline const char* ValueName()
		{
			return "userdata";
		}
		template <> inline const char* ValueName<int>()
		{
			return "number|integer";
		}
		template <> inline const char* ValueName<I64>()
		{
			return "number|integer";
		}
		template <> inline const char* ValueName<UI64>()
		{
			return "number|integer";
		}
		template <> inline const char* ValueName<UI32>()
		{
			return "number|integer";
		}
		template <> inline const char* ValueName<const char*>()
		{
			return "string";
		}
		template <> inline const char* ValueName<bool>()
		{
			return "boolean";
		}

		template <> inline const char* ValueName<float>()
		{
			return "number|float";
		}

		template <> inline const char* ValueName<double>()
		{
			return "number|double";
		}
		///////////// ValueName End

		///////////// Is Begin
		template <typename T> inline bool Is(lua_State* L, int index)
		{
			return lua_islightuserdata(L, index) != 0;
		}
		template <> inline bool Is<int>(lua_State* L, int index)
		{
			return lua_isinteger(L, index) != 0;
		}
		template <> inline bool Is<UI32>(lua_State* L, int index)
		{
			return lua_isinteger(L, index) != 0;
		}
		template <> inline bool Is<UI64>(lua_State* L, int index)
		{
			return lua_isinteger(L, index) != 0;
		}
		template <> inline bool Is<I64>(lua_State* L, int index)
		{
			return lua_isinteger(L, index) != 0;
		}
		template <> inline bool Is<bool>(lua_State* L, int index)
		{
			return lua_isboolean(L, index) != 0;
		}
		template <> inline bool Is<float>(lua_State* L, int index)
		{
			return lua_isnumber(L, index) != 0;
		}
		template <> inline bool Is<double>(lua_State* L, int index)
		{
			return lua_isnumber(L, index) != 0;
		}
		template <> inline bool Is<const char*>(lua_State* L, int index)
		{
			return lua_isstring(L, index) != 0;
		}
		template <> inline bool Is<void*>(lua_State* L, int index)
		{
			return lua_islightuserdata(L, index) != 0;
		}
		///////////// Is End

		///////////// push Begin
		template <typename T> inline void Push(lua_State* L, T value)
		{
			lua_pushlightuserdata(L, value);
		}
		template <> inline void Push(lua_State* L, float value)
		{
			lua_pushnumber(L, value);
		}
		template <typename T> inline void Push(lua_State* L, const T* value)
		{
			lua_pushlightuserdata(L, (T*)value);
		}
		template <> inline void Push(lua_State* L, bool value)
		{
			lua_pushboolean(L, value);
		}
		template <> inline void Push(lua_State* L, const char* value)
		{
			lua_pushstring(L, value);
		}
		template <> inline void Push(lua_State* L, char* value)
		{
			lua_pushstring(L, value);
		}
		template <> inline void Push(lua_State* L, I32 value)
		{
			lua_pushinteger(L, value);
		}
		template <> inline void Push(lua_State* L, UI32 value)
		{
			lua_pushinteger(L, value);
		}
		template <> inline void Push(lua_State* L, UI64 value)
		{
			lua_pushinteger(L, value);
		}
		template <> inline void Push(lua_State* L, void* value)
		{
			lua_pushlightuserdata(L, value);
		}
		///////////// push End

		inline const char* luaTypeToString(int type)
		{
			switch (type)
			{
			case LUA_TNUMBER: return "number";
			case LUA_TBOOLEAN: return "boolean";
			case LUA_TFUNCTION: return "function";
			case LUA_TLIGHTUSERDATA: return "light userdata";
			case LUA_TNIL: return "nil";
			case LUA_TSTRING: return "string";
			case LUA_TTABLE: return "table";
			case LUA_TUSERDATA: return "userdata";
			}
			return "Unknown";
		}
		
		inline void argError(lua_State* L, int index, const char* expected_type)
		{
			char buf[128];
			strcpy(buf, "expected ");
			strcat(buf, expected_type);
			strcat(buf, ", got ");
			int type = lua_type(L, index);
			strcat(buf, LuaWrapper::luaTypeToString(type));
			luaL_argerror(L, index, buf);
		}
		
		template <typename T> void argError(lua_State* L, int index)
		{
			argError(L, index, ValueName<T>());
		}
		
		template <typename T> T checkArg(lua_State* L, int index)
		{
			if (!Is<T>(L, index))
			{
				argError<T>(L, index);
			}
			return Value<T>(L, index);
		}
		
		inline void checkTableArg(lua_State* L, int index)
		{
			if (!lua_istable(L, index))
			{
				argError(L, index, "table");
			}
		}
		
		template <typename T>
		inline void getOptionalField(lua_State* L, int idx, const char* field_name, T* out)
		{
			if (lua_getfield(L, idx, field_name) != LUA_TNIL && Is<T>(L, -1))
			{
				*out = Value<T>(L, -1);
			}
			lua_pop(L, 1);
		}

	}
}