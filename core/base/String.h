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
#ifdef _WIN32
			static char szBuf[1024 * 100];
			static WCHAR wszBuf[1024 * 100] = { 0 };

			int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wszBuf, sizeof(wszBuf));
			OutputDebugStringW(wszBuf);
			OutputDebugStringA("\n");

			WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), nullptr, FALSE);

			return szBuf;
#else 
			return str;
#endif
		}
	};

	
	template<int N>
	class StaticString {
	public:
		StaticString() {  }
		StaticString(char ch) {
			size = 1;
			data[0] = ch;
		}
		StaticString(const char* buff) {
			size = strlen(buff);
			memcpy(data, buff, size);
		}
		StaticString(const char* buff, int len) {
			size = strlen(buff);
			size = Min(size, len);
			memcpy(data, buff, size);
		}
		StaticString(const std::string& rhs) {
			size = rhs.size();
			memcpy(data, rhs.c_str(), size);
		}

		template<typename... Args>
		StaticString(Args... args) {
			int tmp[] = { (Cat(args), 0)... };
			(void)tmp;
		}

		StaticString& Cat(char ch) {
			data[size++] = ch;
			return *this;
		}

		StaticString& Cat(const char* buff) {
			strcat(data, buff);
			size += strlen(buff);
			return *this;
		}

		StaticString& Cat(const std::string& rhs) {
			strcat(data, rhs.c_str());
			size += rhs.size();
			return *this;
		}

		StaticString& Cat(const StaticString& rhs) {
			strcat(data, rhs.data);
			size += rhs.size;
			return *this;
		}

		char Last() {
			if (size == 0) return 0;
			return data[size - 1];
		}
		char First() {
			if (size == 0) return 0;
			return data[0];
		}

		void RemoveLast() {
			if (size == 0) return;
			size--;
			data[size] = 0;
		}

		void RemoveFirst() {
			if (size == 0) return;
			size--;
			memmove(data, data + 1, size);
			data[size] = 0;
		}

		bool Empty() {
			return size == 0;
		}

		int Size() {
			return size;
		}

		int Find(char ch) {
			for (int i = 0; i < size; ++i) {
				if (data[i] == ch) return i;
			}
			return -1;
		}

		int RFind(char ch) {
			for (int i = size - 1; i >= 0; --i) {
				if (data[i] == ch) return i;
			}
			return -1;
		}

		void Replace(char ch, char rep) {
			for (int i = size - 1; i >= 0; --i) {
				if (data[i] == ch) data[i] = rep;
			}
		}

		StaticString& Clear() {
			data[0] = 0;
			size = 0;
			return *this;
		}

		StaticString Sub(int start = 0, int len = 0) {
			if (len == 0) len = N - 1;
			return StaticString(start, len);
		}

		StaticString& operator = (const char* rhs) {
			size = strlen(rhs);
			memcpy(data, rhs, size);
			data[size] = 0;
			return *this;
		}

		StaticString& operator = (StaticString& rhs) {
			size = rhs.size;
			memcpy(data, rhs.data, size);
			data[size] = 0;
			return *this;
		}

		StaticString operator + (StaticString& rhs) {
			StaticString ret = *this;
			return ret += rhs;
		}

		StaticString& operator += (StaticString& rhs) {
			return Cat(rhs);
		}

		bool operator < (const char* rhs) {
			return strcmp(data, rhs) < 0;
		}

		bool operator < (const std::string& rhs) {
			return strcmp(data, rhs.c_str()) < 0;
		}

		bool operator < (const StaticString& rhs) {
			return strcmp(data, rhs.data) < 0;
		}

		bool operator == (const StaticString& rhs) {
			return strcmp(data, rhs.data) == 0;
		}
		bool operator == (const char* buff) {
			return strcmp(data, buff) == 0;
		}
		bool operator == (const std::string& rhs) {
			return strcmp(data, rhs.c_str()) == 0;
		}

		bool operator != (const StaticString& rhs) {
			return strcmp(data, rhs.data) != 0;
		}
		bool operator != (const char* buff) {
			return strcmp(data, buff) != 0;
		}
		bool operator != (std::string& rhs) {
			return strcmp(data, rhs.c_str()) != 0;
		}

		char operator [] (int idx) {
			return data[idx];
		}
	public:
		char data[N] = { 0 };
		int size = 0;
	};

	class PtrString : public StaticString<65> {
	public:
		PtrString(void* ptr) {
			size = sprintf(data, "##%p", ptr);
		}
	};
}