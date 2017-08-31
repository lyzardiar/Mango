#pragma once

#include "core/base/String.h"
#include <string>

namespace RE {
	class Path {
		typedef StaticString<256> DataType;
	public:
		template<typename... Args>
		Path(Args... args) {
			int tmp[] = {1, (Cat(args), 0)... };
			(void)tmp;
		}

		Path(const char* path) {
			Cat(path);
		}

		Path& Cat(const char* path) {
			return Cat(DataType(path));
		}

		Path& Cat(std::string& path) {
			return Cat(DataType(path));
		}

		Path& Cat(Path& path) {
			return Cat(path.data);
		}

		Path& Cat(DataType& path) {
			if (data.size == 0) {
				data = path;
			}
			else {
				while (path.First() == '/' || path.First() == '\\') {
					path.RemoveFirst();
				}
				data.Cat(path);
			}
			if (data.Last() != '/' || data.Last() != '\\') {
				data.Cat('/');
			}
			return *this;
		}

		bool Empty() {
			return data.size == 0;
		}

		int Size() {
			return data.size;
		}

		const char* Bytes() {
			return data.data;
		}

		Path operator + (const char* path) {
			return Path(data, path);
		}

		Path operator + (DataType& path) {
			return Path(data, path);
		}

		Path operator + (Path& path) {
			return Path(data, path);
		}

		Path& operator = (Path& path) {
			data = path.data;
			return *this;
		}

		Path& operator = (std::string& path) {
			data.Clear().Cat(path);
			return *this;
		}

	protected:
		void init();

	public:
		DataType data;
	};
}