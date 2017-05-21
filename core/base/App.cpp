#include "App.h"

#include "windows.h"

RE::App RE::App::instance;

std::string RE::App::AppPath;

bool RE::App::init() {
	if (_inited) return true;

#ifdef _WIN32    
	char full_path[_MAX_PATH + 1];
	::GetModuleFileNameA(nullptr, full_path, _MAX_PATH + 1);
	AppPath = full_path;
	AppPath = AppPath.substr(0, AppPath.rfind("\\") + 1);
#else 
	//Todo ios android
#endif

	_inited = true;
	return true;
}
