#pragma once

#ifdef _WIN32
#include "windows.h"
#endif

#include <string>
#include <algorithm>
#include <stdarg.h>

#include "Types.h"

namespace RE {
	class String : public std::string {
	public:
		static String Replace(const String& str, const String& src, const String& dst) {
			auto pos = 0;
			auto srclen = src.size();
			auto dstlen = dst.size();
			String ret = str;
			while ((pos = ret.find(src, pos)) != String::npos) {
				ret.replace(pos, srclen, dst);
				pos += dstlen;
			}
			return std::move(ret);
		}

		static String Format(const char* format, ...) {
			const int MAX_STRING_LENGTH = 1024 * 100;
			char buf[MAX_STRING_LENGTH];

			va_list ap;
			va_start(ap, format);
			vsnprintf(buf, MAX_STRING_LENGTH, format, ap);
			va_end(ap);

			return buf;
		}

		static String Trim(const std::string& str) {
			if (str.empty()) {
				return str;
			}

			String ret = str;
			ret.erase(0, ret.find_first_not_of(" "));
			ret.erase(ret.find_last_not_of(" ") + 1);
			return std::move(ret);
		}

		static String Trim(const char* str) {
			String ret = str;
			return Trim(ret);
		}

		static String ToLower(const String& str) {
			String ret = str;
			std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
			return std::move(ret);
		}

		static std::string ToUpper(const String& str) {
			String ret = str;
			std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
			return std::move(ret);
		}

		static String UTF8ToGBKWin32(const String& str) {
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

	public:
		String()
			:std::string()
		{

		}

		template<typename... Args>
		String(Args... args) {
			int tmp[] = { (Cat(args), 0)... };
			(void)tmp;
		}

		String& Cat(char ch) {
			*this += ch;
			return *this;
		}

		String& Cat(int num) {
			char buff[50];
			sprintf(buff, "%d", num);
			Cat(buff);
			return *this;
		}

		String& Cat(const char* buff) {
			*this += buff;
			return *this;
		}

		String& Cat(const std::string& rhs) {
			*this += rhs;
			return *this;
		}

		char* Buff() {
			return (char*)c_str();
		}

		String& Replace(String src, String dst) {
			*this = Replace(*this, src, dst);
			return *this;
		}

		char Last() {
			if (empty()) return 0;
			return back();
		}
		char First() {
			if (empty()) return 0;
			return (*this)[0];
		}

		void RemoveLast() {
			if (empty()) return;
			this->erase(size() - 1);
		}

		void RemoveFirst() {
			if (empty()) return;
			this->erase(0);
		}

		String& Title() {
			if (empty()) *this;
			auto self = *this;
			self[0] = toupper(self[0]);
			return self;
		}

		bool Empty() {
			return empty();
		}

		int Size() {
			return size();
		}

		int Find(char ch) {
			int sz = (int)size();
			auto& self = (*this);
			for (int i = 0; i < sz; ++i) {
				if (self[i] == ch) return i;
			}
			return -1;
		}

		int RFind(char ch) {
			int sz = (int)size();
			auto& self = (*this);
			for (int i = sz - 1; i >= 0; --i) {
				if (self[i] == ch) return i;
			}
			return -1;
		}

		void Replace(char ch, char rep) {
			int sz = (int)size();
			auto& self = (*this);
			for (int i = sz - 1; i >= 0; --i) {
				if (self[i] == ch) self[i] = rep;
			}
		}

		bool EndWith(const char* tail) {
			auto len = strlen(tail);
			int sz = (int)size();
			if ((int)len > sz) return false;
			return strcmp(tail, c_str() + (sz - len)) == 0;
		}

		String& Clear() {
			*this = "";
			return *this;
		}

		String Sub(int start = 0, int len = INT_MAX) {
			return substr(0, len);
		}
		/*
		String& operator = (const char* rhs) {
			size = strlen(rhs);
			memcpy(data, rhs, size);
			data[size] = 0;
			return *this;
		}

		String& operator = (const std::string& rhs) {
			size = rhs.size();
			memcpy(data, rhs.c_str(), size);
			data[size] = 0;
			return *this;
		}

		String& operator = (const String& rhs) {
			size = rhs.size;
			memcpy(data, rhs.data, size);
			data[size] = 0;
			return *this;
		}

		StaticString operator + (const StaticString& rhs) {
			StaticString ret = *this;
			return ret += rhs;
		}

		StaticString& operator += (const StaticString& rhs) {
			return Cat(rhs);
		}

		StaticString& operator += (char rhs) {
			return Cat(rhs);
		}

		StaticString& operator += (const char* rhs) {
			return Cat(rhs);
		}

		StaticString& operator += (const std::string& rhs) {
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

		bool operator == (const StaticString& rhs) const {
			return strcmp(data, rhs.data) == 0;
		}
		bool operator == (const char* buff) const {
			return strcmp(data, buff) == 0;
		}
		bool operator == (const std::string& rhs) const {
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
		}*/
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
		StaticString(const String& rhs) {
			size = rhs.size();
			memcpy(data, rhs.c_str(), size);
		}
		StaticString(const StaticString& rhs) {
			size = rhs.size;
			memcpy(data, rhs.data, size);
		}
		StaticString(const StaticString&& rhs) {
			size = rhs.size;
			memcpy(data, rhs.data, size);
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

		StaticString& Cat(int num) {
			char buff[50];
			sprintf(buff, "%d", num);
			Cat(buff);
			return *this;
		}

		StaticString& Cat(const char* buff) {
			strncat(data, buff, N - size);
			size += strlen(buff);
			if (size > N) size = N;
			return *this;
		}

		StaticString& Cat(const String& rhs) {
			strncat(data, rhs.c_str(), N - size);
			size += rhs.size();
			if (size > N) size = N;
			return *this;
		}

		StaticString& Cat(const StaticString& rhs) {
			strncat(data, rhs.data, N - size);
			size += rhs.size;
			if (size > N) size = N;
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

		StaticString& Title() {
			data[0] = toupper(data[0]);
			return *this;
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

		bool EndWith(const char* tail) {
			auto len = strlen(tail);
			if ((int)len > size) return false;
			return strcmp(tail, data + (size - len)) == 0;
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

		StaticString& operator = (const String& rhs) {
			size = rhs.size();
			memcpy(data, rhs.c_str(), size);
			data[size] = 0;
			return *this;
		}

		StaticString& operator = (const StaticString& rhs) {
			size = rhs.size;
			memcpy(data, rhs.data, size);
			data[size] = 0;
			return *this;
		}

		StaticString operator + (const StaticString& rhs) {
			StaticString ret = *this;
			return ret += rhs;
		}

		StaticString& operator += (const StaticString& rhs) {
			return Cat(rhs);
		}

		StaticString& operator += (char rhs) {
			return Cat(rhs);
		}

		StaticString& operator += (const char* rhs) {
			return Cat(rhs);
		}

		StaticString& operator += (const String& rhs) {
			return Cat(rhs);
		}

		bool operator < (const char* rhs) {
			return strcmp(data, rhs) < 0;
		}

		bool operator < (const String& rhs) {
			return strcmp(data, rhs.c_str()) < 0;
		}

		bool operator < (const StaticString& rhs) {
			return strcmp(data, rhs.data) < 0;
		}

		bool operator == (const StaticString& rhs) const {
			return strcmp(data, rhs.data) == 0;
		}
		bool operator == (const char* buff) const {
			return strcmp(data, buff) == 0;
		}
		bool operator == (const String& rhs) const {
			return strcmp(data, rhs.c_str()) == 0;
		}

		bool operator != (const StaticString& rhs) {
			return strcmp(data, rhs.data) != 0;
		}
		bool operator != (const char* buff) {
			return strcmp(data, buff) != 0;
		}
		bool operator != (String& rhs) {
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