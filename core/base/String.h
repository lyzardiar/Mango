#pragma once

#ifdef _WIN32
#include "windows.h"
#endif

#include <string>
#include <algorithm>
#include <stdarg.h>

#include "Types.h"

namespace RE {
	class String {
	public:
		static std::string Replace(const std::string& str, const std::string& src, const std::string& dst) {
			auto pos = 0;
			auto srclen = src.size();
			auto dstlen = dst.size();
			std::string ret = str;
			while ((pos = ret.find(src, pos)) != std::string::npos) {
				ret.replace(pos, srclen, dst);
				pos += dstlen;
			}
			return std::move(ret);
		}

		static std::string Format(const char* format, ...) {
			const int MAX_STRING_LENGTH = 1024 * 100;
			char buf[MAX_STRING_LENGTH];

			va_list ap;
			va_start(ap, format);
			vsnprintf(buf, MAX_STRING_LENGTH, format, ap);
			va_end(ap);

			return buf;
		}

		static std::string Trim(const std::string& str) {
			if (str.empty()) {
				return str;
			}

			std::string ret = str;
			ret.erase(0, ret.find_first_not_of(" "));
			ret.erase(ret.find_last_not_of(" ") + 1);
			return std::move(ret);
		}

		static std::string Trim(const char* str) {
			std::string ret = str;
			return Trim(ret);
		}

		static std::string ToLower(const std::string& str) {
			std::string ret = str;
			std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
			return std::move(ret);
		}

		static std::string ToUpper(const std::string& str) {
			std::string ret = str;
			std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
			return std::move(ret);
		}

		static std::string UTF8ToGBKWin32(const std::string& str) {
			static char szBuf[1024 * 100];
			static WCHAR wszBuf[1024 * 100] = { 0 };

			int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wszBuf, sizeof(wszBuf));
			OutputDebugStringW(wszBuf);
			OutputDebugStringA("\n");

			WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), nullptr, FALSE);

			return szBuf;
		}
	};
}