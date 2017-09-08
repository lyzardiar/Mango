#pragma once

#include <functional>
#include "imgui/imgui.h"

namespace RE {
	class Window {
	public:
		static Window* instance;
	public:
		Window();
		~Window();

	public:
		bool loop();
		
	public:
		std::function<bool(float dt)> RenderHandle = nullptr;

	protected:
		bool initGL();
		bool close();

	protected:
		ImVec4 _clearColor;
	};

}

