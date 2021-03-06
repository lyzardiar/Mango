#include "core/platform/Window.h"

#include <stdio.h>
#include <string>

#include <imgui/imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "renderer/GLProgram.h"
#include "renderer/Texture2D.h"
#include "image/PngDecoder.h"
#include "base/Time.h"

using namespace RE;


RE::Window* RE::Window::instance = nullptr;
GLFWwindow* window = nullptr;

static void error_callback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}

bool Window::initGL() {
	if (window != nullptr) {
		close();
	}

	// Setup window
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return false;
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	window = glfwCreateWindow(1334, 750, "Mango", NULL, NULL);
	glfwMakeContextCurrent(window);
	gl3wInit();

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);

	return true;
}

bool Window::close() {
	if (window == nullptr) return true;

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
	window = nullptr;

	return true;
}

bool Window::loop() {
	double lastTime = Time::Clock();
	double interval = 0.0166;

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		double curTime = Time::Clock();
		if (curTime - lastTime >= interval) {
			glfwPollEvents();
			ImGui_ImplGlfwGL3_NewFrame();
			// Rendering
			glViewport(0, 0, (GLsizei)ImGui::GetIO().DisplaySize.x, (GLsizei)ImGui::GetIO().DisplaySize.y);
			glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
			glClear(GL_COLOR_BUFFER_BIT);

			if (RenderHandle != nullptr) {
				RenderHandle(ImGui::GetIO().DeltaTime);
			}

			ImGui::Render();
			glfwSwapBuffers(window);

			lastTime = curTime;
		}
		else {
			Sleep(DWORD((interval - (curTime - lastTime)) * 1000));
		}
	}
	return true;
}

Window::Window() {
	instance = this;
	initGL();
}

Window::~Window() {
	close();
	instance = nullptr;
}
