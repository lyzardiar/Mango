#pragma once

#include <string>
#include <vector>

#include "core/base/Macros.h"
#include "core/platform/Platform.h"
#include "core/base/Data.h"
#include "core/base/Path.h"

namespace RE {
	class FileUtils {
	public:
		static FileUtils* getInstance();

	public:
		bool IsFileExists(Path& path);
		bool IsRealPath(Path& path);
		Path RealPath(Path& path);
		void AddSearchDir(Path path);
		void ClearSearchDirs();
		Data GetData(Path& path);
		
	protected:
		std::vector<Path> _searchDirs;
	};
}