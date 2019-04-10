#include "GLProgramSystem.h"
#include "core/base/Data.h"
#include "core/platform/FileUtils.h"


void RE::GLProgramSystem::InitDefault() {
	Add("Default", "shaders/default.vert", "shaders/default.frag");
}


RE::GLProgramSystem RE::GLProgramSystem::instance;

RE::GLProgram* RE::GLProgramSystem::Add(const char* name, const char* vertFile, const char* fragFile) {
	return Add(name, Path(vertFile), Path(fragFile));
}

RE::GLProgram* RE::GLProgramSystem::Add(const char* name, Path vertFile, Path fragFile) {
	Data vertdata = FileUtils::getInstance()->GetData(vertFile);
	Data fragdata = FileUtils::getInstance()->GetData(fragFile);
	return AddWithBuff(name, (char*)vertdata.getBytes(), (char*)fragdata.getBytes());
}

RE::GLProgram* RE::GLProgramSystem::AddWithBuff(const char* name, const char* vert, const char* frag) {
	auto old = programs[name];
	if (old != nullptr) {
		old->Release();
	}
	auto cur = new GLProgram();
	if (cur->InitWithBuff(vert, frag)) {
		programs[name] = cur;
	}
	else {
		delete cur;
		cur = nullptr;
	}
	return cur;
}

RE::GLProgram* RE::GLProgramSystem::Get(const char* name) {
	static std::string progName;
	progName.assign(name);
	return Get(progName);
}

RE::GLProgram* RE::GLProgramSystem::operator[](const char* name) {
	return Get(name);
}

RE::GLProgram* RE::GLProgramSystem::Get(const std::string& name) {
	return programs[name];
}

RE::GLProgram* RE::GLProgramSystem::operator[](const std::string& name) {
	return Get(name);
}
