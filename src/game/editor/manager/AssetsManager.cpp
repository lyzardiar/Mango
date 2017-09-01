#include "AssetsManager.h"

#include "platform/win32/FileHelperWin32.h"

RE::AssetsManager RE::AssetsManager::instance;

void RE::AssetsManager::ScanFold(const char* fold) {
	FileDirWin fdir(".");
	curDir = fdir.GetDirName();

	fdir.BeginBrowse("*.*");

	auto& vfiles = fdir.Files();

	PathType spath;
	for (auto& path : vfiles) {
		path = String::Replace(path, curDir.data, ""); 		
		spath = String::Replace(path, "\\", "/");
		AddFile(spath);
	}
	
}

void RE::AssetsManager::AddFile(PathType& file) {
	if (file.EndWith(".png") || file.EndWith(".jpg")) {
		addCatagrayFile(imageFiles, file);
	}
	else if (file.EndWith(".lua")) {
		addCatagrayFile(scriptFiles, file);
	}
	else if (file.EndWith(".frag") || file.EndWith(".vert")) {
		addCatagrayFile(shaderFiles, file);
	}
	addCatagrayFile(files, file);
}

bool RE::AssetsManager::addCatagrayFile(Array<PathType>& container, const PathType& file) {
	bool ret = false;
	if (container.Find(file) == -1) {
		container.Push(file);
		ret = true;
	}
	return ret;
}
