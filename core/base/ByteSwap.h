#pragma once

#include "core/base/Types.h"

namespace RE {

	template <typename From, typename To>
	class TypeAliaser {
	public:
		TypeAliaser(From val) : fromType(val) {}
		To& Get() { return toType; }
		union {
			From fromType;
			To toType;
		};
	};

	class ByteSwapRunner {
	public:
		static UI16 Run(UI16 val) {
			return (val >> 8) | (val << 8);
		}

		static UI32 Run(UI32 val) {
			return ((val >> 24) & 0x000000ff) | ((val >> 8) & 0x0000ff00) |
				((val << 8) & 0x00ff0000) | ((val << 24) & 0xff000000);
		}

		static UI64 Run(UI64 val) {
			return ((val >> 56) & 0x00000000000000ff) | ((val >> 40) & 0x000000000000ff00) |
				((val >> 24) & 0x0000000000ff0000) | ((val >> 8) & 0x00000000ff000000) |
				((val << 8) & 0x000000ff00000000) | ((val << 24) & 0x0000ff0000000000) |
				((val << 40) & 0x00ff000000000000) | ((val << 56) & 0xff00000000000000);
		}
	};

	template <typename T, int size> class ByteSwapProxy;
	template <typename T> class ByteSwapProxy<typename T, 2> {
	public:
		T Get(T val) {
			UI16 ret = ByteSwapRunner.Run(TypeAliaser<T, UI16>(val).Get());
			return TypeAliaser<UI16, T>(ret).Get();
		}
	};
	template <typename T> class ByteSwapProxy<typename T, 4> {
	public:
		T Get(T val) {
			UI32 ret = ByteSwapRunner.Run(TypeAliaser<T, UI32>(val).Get());
			return TypeAliaser<UI32, T>(ret).Get();
		}
	};
	template <typename T> class ByteSwapProxy<typename T, 8> {
	public:
		T Get(T val) {
			UI64 ret = ByteSwapRunner.Run(TypeAliaser<T, UI64>(val).Get());
			return TypeAliaser<UI64, T>(ret).Get();
		}
	};

	template <typename T>
	T ByteSwap(T& val) {
		return ByteSwapProxy<T, sizeof(T)>().Get(val);
	}
}