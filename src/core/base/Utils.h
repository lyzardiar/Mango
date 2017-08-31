#pragma once

#include "core/base/Types.h"

#ifdef _WIN32
#include <guiddef.h>
#include <combaseapi.h>
#endif

namespace RE {
	namespace Utils {
		StaticString<65> GenUUID() {
			StaticString<65> buffer;
#ifdef _WIN32
			GUID guid;

			if (CoCreateGuid(&guid)) {
				fprintf(stderr, "create guid error\n");
				return buffer;
			}
			_snprintf(buffer.data, sizeof(buffer),
				"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
				guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2],
				guid.Data4[3], guid.Data4[4], guid.Data4[5],
				guid.Data4[6], guid.Data4[7]);
#endif
			return buffer;
		}
	}
}