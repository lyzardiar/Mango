#include "FileUtils.h"

#include "core/base/File.h"
#include "core/base/App.h"

using namespace RE;

FileUtils* FileUtils::getInstance() {
	static FileUtils* instance = nullptr;
	if (instance == nullptr) {
		instance = new FileUtils();

		App::instance.init();

		instance->AddSearchDir(App::AppPath + "../../../../game/res");
		instance->AddSearchDir(App::AppPath);
	}

	return instance;
}


Path FileUtils::RealPath(Path& path) {
	if (IsRealPath(path)) return path;

	for (auto& dir : _searchDirs) {
		auto rpath = dir + path;
		if (FileState(rpath).size > 0) {
			return rpath;
		}
	}
	return path;
}

void FileUtils::AddSearchDir(Path path) {
	if (path.Empty()) return;
	_searchDirs.push_back(path);
}

bool RE::FileUtils::IsFileExists(Path& path) {
	return FileState(RealPath(path)).size > 0;
}

bool RE::FileUtils::IsRealPath(Path& path)
{
#ifdef _WIN32 
	if (path.Size() < 2) return false;
	return isalpha(path[0]) && path[1] == ':';
#else 
	if (path.Empty()) return false;
	return path[0] == '/';
#endif
}

void RE::FileUtils::ClearSearchDirs() {
	_searchDirs.clear();
}

RE::Data RE::FileUtils::GetData(Path& path) {
	File file(RealPath(path));

	Data data;
	data.fastSet(file.Buff(), file.Size());

	file.DropBuff();
	return data;
}
