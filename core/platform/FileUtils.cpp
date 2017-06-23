#include "FileUtils.h"

#include "core/base/File.h"
#include "core/base/App.h"

using namespace RE;

FileUtils* FileUtils::getInstance() {
	static FileUtils instance;

	App::instance.init();

	instance.addSearchDir(App::AppPath + "../../../../game/res");
	instance.addSearchDir(App::AppPath);

	return &instance;
}

std::string FileUtils::realPath(std::string& path) {
	if (isRealPath(path)) return path;

	for (auto& dir : _searchDirs) {
		auto rpath = dir + path;
		if (FileState(rpath).size > 0) {
			return rpath;
		}
	}
	return path;
}

void FileUtils::addSearchDir(std::string& path) {
	if (path.empty()) return;

	char ch = path[path.size() - 1];
	if (ch != '/' || ch != '\\') path += "/";
	_searchDirs.push_back(path);
}

bool RE::FileUtils::isFileExists(std::string& path) {
	return FileState(path).size > 0;
}

bool RE::FileUtils::isFileExists(const char* path) {
	std::string filename = path;
	return isFileExists(filename);
}

bool RE::FileUtils::isRealPath(std::string& path)
{
#ifdef _WIN32 
	if (path.size() < 2) return false;
	return isalpha(path[0]) && path[1] == ':';
#else 
	if (path.empty()) return false;
	return path[0] == '/';
#endif
}

void RE::FileUtils::clearSearchDirs() {
	_searchDirs.clear();
}

RE::Data RE::FileUtils::getData(std::string& path) {
	std::string fullpath = realPath(path);
	File file(fullpath);

	Data data;
	data.fastSet(file.Buff(), file.Size());

	file.DropBuff();
	return data;
}

RE::Data RE::FileUtils::getData(const char* path) {
	std::string filepath(path);
	return getData(filepath);
}
