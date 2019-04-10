#pragma once

#include <unordered_map>
#include "renderer/GLProgram.h"
#include <string>
#include "base/Path.h"

namespace RE {
	class GLProgramSystem {
	public:
		static GLProgramSystem instance;
	public:
		GLProgram* Add(const char* name, const char* vertFile, const char* fragFile);
		GLProgram* Add(const char* name, Path vertFile, Path fragFile);
		GLProgram* AddWithBuff(const char* name, const char* vert, const char* frag);

		GLProgram* Get(const char* name);
		GLProgram* operator[](const char* name);
		GLProgram* Get(const std::string& name);
		GLProgram* operator[](const std::string& name);

		void InitDefault();
	public:
		std::unordered_map<std::string, GLProgram*> programs;
	};
}