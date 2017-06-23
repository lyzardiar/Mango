#ifndef __LUA_WRAPPER_H__
#define __LUA_WRAPPER_H__

#include "lua.hpp"
#include "core/base/Types.h"

namespace RE {
	namespace LuaWrapper {

		///////////// Value Begin
		template <typename T> inline T	Value(lua_State* L, int index) 	{ return (T)lua_touserdata(L, index); }
		template <> inline int			Value(lua_State* L, int index) 	{ return (int)lua_tointeger(L, index); }
		template <> inline I64			Value(lua_State* L, int index) 	{ return (I64)lua_tointeger(L, index); }
		template <> inline UI32			Value(lua_State* L, int index) 	{ return (UI32)lua_tointeger(L, index); }
		template <> inline UI64			Value(lua_State* L, int index) 	{ return (UI64)lua_tointeger(L, index); }
		template <> inline bool			Value(lua_State* L, int index) 	{ return lua_toboolean(L, index) != 0; }
		template <> inline float		Value(lua_State* L, int index) 	{ return (float)lua_tonumber(L, index); }
		template <> inline const char*	Value(lua_State* L, int index) 	{ return lua_tostring(L, index); }
		template <> inline void*		Value(lua_State* L, int index) 	{ return lua_touserdata(L, index); }
		///////////// Value End

		///////////// ValueName Begin
		template <typename T> inline const char* ValueName() 	{ return "userdata"; }
		template <> inline const char* ValueName<int>() 		{ return "number|integer"; }
		template <> inline const char* ValueName<I64>() 		{ return "number|integer"; }
		template <> inline const char* ValueName<UI64>() 		{ return "number|integer"; }
		template <> inline const char* ValueName<UI32>() 		{ return "number|integer"; }
		template <> inline const char* ValueName<const char*>() { return "string"; }
		template <> inline const char* ValueName<bool>() 		{ return "boolean"; }
		template <> inline const char* ValueName<float>() 		{ return "number|float"; }
		template <> inline const char* ValueName<double>() 		{ return "number|double"; }
		///////////// ValueName End

		///////////// Is Begin
		template <typename T> inline bool Is(lua_State* L, int index) 	{ return lua_islightuserdata(L, index) != 0; }
		template <> inline bool Is<int>(lua_State* L, int index) 		{ return lua_isinteger(L, index) != 0; }
		template <> inline bool Is<UI32>(lua_State* L, int index) 		{ return lua_isinteger(L, index) != 0; }
		template <> inline bool Is<UI64>(lua_State* L, int index) 		{ return lua_isinteger(L, index) != 0; }
		template <> inline bool Is<I64>(lua_State* L, int index) 		{ return lua_isinteger(L, index) != 0; }
		template <> inline bool Is<bool>(lua_State* L, int index) 		{ return lua_isboolean(L, index) != 0; }
		template <> inline bool Is<float>(lua_State* L, int index) 		{ return lua_isnumber(L, index) != 0; }
		template <> inline bool Is<double>(lua_State* L, int index) 	{ return lua_isnumber(L, index) != 0; }
		template <> inline bool Is<const char*>(lua_State* L, int index){ return lua_isstring(L, index) != 0; }
		template <> inline bool Is<void*>(lua_State* L, int index) 		{ return lua_islightuserdata(L, index) != 0; }
		///////////// Is End

		///////////// push Begin
		template <typename T> inline void Push(lua_State* L, T value) 		{ lua_pushlightuserdata(L, value); }
		template <> inline void Push(lua_State* L, float value) 			{ lua_pushnumber(L, value); }
		template <typename T> inline void Push(lua_State* L, const T* value){ lua_pushlightuserdata(L, (T*)value); }
		template <> inline void Push(lua_State* L, bool value) 				{ lua_pushboolean(L, value); }
		template <> inline void Push(lua_State* L, const char* value) 		{ lua_pushstring(L, value); }
		template <> inline void Push(lua_State* L, char* value) 			{ lua_pushstring(L, value); }
		template <> inline void Push(lua_State* L, I32 value) 				{ lua_pushinteger(L, value); }
		template <> inline void Push(lua_State* L, UI32 value) 				{ lua_pushinteger(L, value); }
		template <> inline void Push(lua_State* L, UI64 value) 				{ lua_pushinteger(L, value); }
		template <> inline void Push(lua_State* L, void* value) 			{ lua_pushlightuserdata(L, value); }
		///////////// push End

		///////////// size of arg Begin
		template <typename R, typename... Args> constexpr int Length(R(*f)(Args...)) 								{ return sizeof...(Args); }
		template <typename R, typename... Args> constexpr int Length(R(*f)(lua_State*, Args...)) 					{ return sizeof...(Args); }
		template <typename R, typename C, typename... Args> constexpr int Length(R(C::*f)(Args...)) 				{ return sizeof...(Args); }
		template <typename R, typename C, typename... Args> constexpr int Length(R(C::*f)(Args...) const) 			{ return sizeof...(Args); }
		template <typename R, typename C, typename... Args> constexpr int Length(R(C::*f)(lua_State*, Args...))		{ return sizeof...(Args); }
		template <typename R, typename C, typename... Args> constexpr int Length(R(C::*f)(lua_State*, Args...) const){ return sizeof...(Args); }
		///////////// size of arg End

		///////////// remove reference Begin
		template <class T> struct remove_reference 		{ using type = T; };
		template <class T> struct remove_reference<T&> 	{ using type = T; };
		template <class T> struct remove_reference<T&&> { using type = T; };
		template <class T> struct remove_const 			{ using type = T; };
		template <class T> struct remove_const<const T> { using type = T; };
		template <class T> struct remove_volatile 		{ using type = T; };
		template <class T> struct remove_volatile<volatile T> { using type = T; };
		template <class T> struct remove_cv_reference 	{ using type =  typename remove_const<typename remove_volatile<typename remove_reference<T>::type>::type>::type; };
		///////////// remove reference End

		///////////// build indices Begin
		template <int... T>
		struct Indices {};

		template <int offset, int size, int... T>
		struct build_indices { using result = typename build_indices<offset, size - 1, size + offset, T...>::result; };
		
		template <int offset, int... T>
		struct build_indices<offset, 0, T...> { using result = Indices<T...>; };
		///////////// build indices End

		///////////// Caller Begin
		template <typename T, int index>
		typename remove_cv_reference<T>::type convert(lua_State* L) {
			return checkArg<typename remove_cv_reference<T>::type>(L, index);
		}

		template <typename T> struct Caller;
		
		template <int... indices>
		struct Caller<Indices<indices...>> {
			template <typename R, typename... Args>
			static int callFunction(R(*f)(Args...), lua_State* L) {
				R v = f(convert<Args, indices>(L)...);
				push(L, v);
				return 1;
			}
			
			template <typename... Args>
			static int callFunction(void(*f)(Args...), lua_State* L) {
				f(convert<Args, indices>(L)...);
				return 0;
			}
			
			template <typename R, typename... Args>
			static int callFunction(R(*f)(lua_State*, Args...), lua_State* L) {
				R v = f(L, convert<Args, indices>(L)...);
				push(L, v);
				return 1;
			}
			
			template <typename... Args>
			static int callFunction(void(*f)(lua_State*, Args...), lua_State* L) {
				f(L, convert<Args, indices>(L)...);
				return 0;
			}
			
			template <typename C, typename... Args>
			static int callMethod(C* inst, void(C::*f)(lua_State*, Args...), lua_State* L) {
				(inst->*f)(L, convert<Args, indices>(L)...);
				return 0;
			}

			template <typename R, typename C, typename... Args>
			static int callMethod(C* inst, R(C::*f)(lua_State*, Args...), lua_State* L) {
				R v = (inst->*f)(L, convert<Args, indices>(L)...);
				push(L, v);
				return 1;
			}
			
			template <typename R, typename C, typename... Args>
			static int callMethod(C* inst, R(C::*f)(lua_State*, Args...) const, lua_State* L) {
				R v = (inst->*f)(L, convert<Args, indices>(L)...);
				push(L, v);
				return 1;
			}
			
			template <typename C, typename... Args>
			static int callMethod(C* inst, void(C::*f)(Args...), lua_State* L) {
				(inst->*f)(convert<Args, indices>(L)...);
				return 0;
			}
			
			template <typename R, typename C, typename... Args>
			static int callMethod(C* inst, R(C::*f)(Args...), lua_State* L) {
				R v = (inst->*f)(convert<Args, indices>(L)...);
				push(L, v);
				return 1;
			}
			
			template <typename R, typename C, typename... Args>
			static int callMethod(C* inst, R(C::*f)(Args...) const, lua_State* L) {
				R v = (inst->*f)(convert<Args, indices>(L)...);
				push(L, v);
				return 1;
			}
		};
		
		///////////// Caller End

		inline const char* luaTypeToString(int type) {
			switch (type) {
			case LUA_TNUMBER:		return "number";
			case LUA_TBOOLEAN:		return "boolean";
			case LUA_TFUNCTION:		return "function";
			case LUA_TLIGHTUSERDATA:return "light userdata";
			case LUA_TNIL:			return "nil";
			case LUA_TSTRING:		return "string";
			case LUA_TTABLE:		return "table";
			case LUA_TUSERDATA:		return "userdata";
			}
			return "Unknown";
		}
		
		inline void argError(lua_State* L, int index, const char* expected_type) {
			char buf[128];
			strcpy(buf, "expected ");
			strcat(buf, expected_type);
			strcat(buf, ", got ");
			int type = lua_type(L, index);
			strcat(buf, LuaWrapper::luaTypeToString(type));
			luaL_argerror(L, index, buf);
		}
		
		template <typename T> void argError(lua_State* L, int index) {
			argError(L, index, ValueName<T>());
		}
		
		template <typename T> T checkArg(lua_State* L, int index) {
			if (!Is<T>(L, index)) {
				argError<T>(L, index);
			}
			return Value<T>(L, index);
		}
		
		inline void checkTableArg(lua_State* L, int index) {
			if (!lua_istable(L, index)) {
				argError(L, index, "table");
			}
		}
		
		template <typename T>
		inline void getOptionalField(lua_State* L, int idx, const char* field_name, T* out) {
			if (lua_getfield(L, idx, field_name) != LUA_TNIL && Is<T>(L, -1)) {
				*out = Value<T>(L, -1);
			}
			lua_pop(L, 1);
		}

		// static function
		template <typename T, T t> int wrap(lua_State* L) {
			using indices = typename details::build_indices<0, details::arity(t)>::result;
			return details::Caller<indices>::callFunction(t, L);
		}
		
		// class function
		template <typename C, typename T, T t> int wrapMethod(lua_State* L) {
			using indices = typename details::build_indices<1, details::arity(t)>::result;
			auto* inst = checkArg<C*>(L, 1);
			return details::Caller<indices>::callMethod(inst, t, L);
		}

		// for instance
		template <typename C, typename T, T t> int wrapMethodClosure(lua_State* L) {
			using indices = typename details::build_indices<0, details::arity(t)>::result;
			int index = lua_upvalueindex(1);
			if (!Is<T>(L, index)) {
				luaL_error(L, "%s", "Invalid Lua closure.");
				ASSERT(false);
				return 0;
			}
			auto* inst = checkArg<C*>(L, index);
			return details::Caller<indices>::callMethod(inst, t, L);
		}
		
		inline void getCls(lua_State* L, const char* cls) {
			if (lua_getglobal(L, cls) == LUA_TNIL) {
				lua_pop(L, 1);
				lua_newtable(L);
				lua_setglobal(L, cls);
				lua_getglobal(L, cls);
			}
		}

		// register object
		inline void Reg(lua_State* L, const char* cls, const char* var_name, void* value) {
			getCls(L, cls);
			lua_pushlightuserdata(L, value);
			lua_setfield(L, -2, var_name);
			lua_pop(L, 1);
		}
		
		// register constans
		inline void Reg(lua_State* L, const char* cls, const char* var_name, int value) {
			getCls(L, cls);
			lua_pushinteger(L, value);
			lua_setfield(L, -2, var_name);
			lua_pop(L, 1);
		}
		
		// register method
		inline void Reg(lua_State* L, const char* cls, const char* var_name, lua_CFunction fn) {
			getCls(L, cls);
			lua_pushcfunction(L, fn);
			lua_setfield(L, -2, var_name);
			lua_pop(L, 1);
		}

		// register instance 
		inline void Reg(lua_State* L, const char* cls, const char* var_name, lua_CFunction fn, void* system_ptr) {
			getCls(L, cls);
			lua_pushlightuserdata(L, system_ptr);
			lua_pushcclosure(L, fn, 1);
			lua_setfield(L, -2, var_name);
			lua_pop(L, 1);
		}

		inline int executeString(lua_State* L, const char* code) {
			return luaL_dostring(L, code);
		}

		inline int executeFile(lua_State* L, const char* path) {
			return luaL_dofile(L, path);
		}

		void registerLua();
	}
}

#include "kaguya.hpp"

#endif // __LUA_WRAPPER_H__