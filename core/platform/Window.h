#pragma once

namespace RE {
	class Window {
	public:
		Window();
		~Window();

	public:
		bool loop();

	protected:
		bool initGL();
		bool close();
	};

}

