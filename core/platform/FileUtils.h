#pragma once

#include <string>
#include <vector>

#include "core/base/Macros.h"
#include "core/platform/Platform.h"
#include "core/base/Data.h"

namespace RE {
	class FileUtils {
	public:
		static FileUtils* getInstance();

	public:
		bool isFileExists(std::string& path);
		bool isFileExists(const char* path);

		bool isRealPath(std::string& path);

		std::string realPath(std::string& path);

		void addSearchDir(std::string& path);
		void clearSearchDirs();

		Data getData(std::string& path);
		Data getData(const char* path);

	public:
		const std::string App;

	protected:
		std::vector<std::string> _searchDirs;
	};
}