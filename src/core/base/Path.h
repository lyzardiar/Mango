#pragma once

#include "core/base/String.h"
#include <string>

namespace RE {
	class Path {
		typedef String Type;
	public:
		struct HashFunc {
			std::size_t operator()(const Path &key) const {
				using std::size_t;
				using std::hash;
				return hash<const char*>()(key.data.c_str());
			}
		};
		struct EqualFunc {
			bool operator () (const Path &lhs, const Path &rhs) const {
				return lhs.data == rhs.data;
			}
		};

	public:
		template<typename... Args>
		Path(Args... args) {
			int tmp[] = {1, (Cat(args), 0)... };
			(void)tmp;
		}

		Path(const char* path) {
			Cat(path);
		}

		Path(String path) {
			Cat(path);
		}

		Path& Cat(const char* path) {
			return Cat(Type(path));
		}

		Path& Cat(Path& path) {
			return Cat(path.data);
		}

		Path& Cat(Type path) {
			if (data.Empty()) {
				data = path;
			}
			else {
				while (path.First() == '/' || path.First() == '\\') {
					path.RemoveFirst();
				}
				data.Cat("/").Cat(path);
			}
			while (data.Last() == '/' || data.Last() == '\\') {
				data.RemoveLast();
			}
			return *this;
		}

		bool Empty() {
			return data.Empty();
		}

		int Size() {
			return data.Size();
		}

		const char* Bytes() {
			return data.c_str();
		}

		Path operator + (const char* path) {
			return Path(data, path);
		}

		Path operator + (Type& path) {
			return Path(data, path);
		}

		Path operator + (Path& path) {
			return Path(data, path);
		}

		Path& operator += (const char* path) {
			data.Cat(path);
			return *this;
		}

		Path& operator += (Type& path) {
			data.Cat(path);
			return *this;
		}

		Path& operator += (Path& path) {
			data.Cat(path.data);
			return *this;
		}

		char operator [] (int index) {
			return data[index];
		}

		bool operator == (const Path& path) {
			return data == path.data;
		}

		bool operator != (const Path& path) {
			return ! (*this == path);
		}

	protected:
		void init();

	public:
		Type data;
	};
}