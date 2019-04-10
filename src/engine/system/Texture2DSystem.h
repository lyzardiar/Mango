#pragma once
#include "core/base/Array.h"
#include "core/base/String.h"
#include "core/base/Path.h"

#include <string>
#include <unordered_map>

namespace RE {
	class Texture2DSystem {
	public:
		static Texture2DSystem instance;

	public:
		Texture2DSystem();

		void InitDefault();

		class Texture2D* Add(const char* path);
		class Texture2D* Add(const Path& path);

		class Texture2D* operator[](const char* path);
		class Texture2D* operator[](const Path& path);

	public:
		std::unordered_map<Path, class Texture2D*, Path::HashFunc, Path::EqualFunc> textures;
	};
}
