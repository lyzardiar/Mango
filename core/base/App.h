#pragma once

#include <string>

namespace RE {
	class App {
	public:
		static App instance;

		static std::string AppPath;

	public:
		bool init();

	protected:
		bool _inited = false;
	};
}