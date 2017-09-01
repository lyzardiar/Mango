#pragma once
#include "core/base/Array.h"
#include "core/base/String.h"

#include <string>
#include <unordered_map>

namespace RE {
	class Texture2DSystem {
	public:
		static Texture2DSystem instance;

	public:

		class Texture2D* Add(const char* path);
		class Texture2D* Add(const StaticString<128>& path);

	public:
		std::unordered_map<std::string, class Texture2D*> textures;
	};
}
