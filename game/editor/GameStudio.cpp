#include "GameStudio.h"
#include "imgui/imgui.h"
#include "core/platform/Window.h"



RE::GameStudio::GameStudio()
{

}

RE::GameStudio::~GameStudio()
{

}

bool RE::GameStudio::Update(float dt)
{
	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	auto& imIO = ImGui::GetIO();
	ImGui::RootDock(ImVec2(0, 0), ImVec2(imIO.DisplaySize.x, imIO.DisplaySize.y));

	{
		static bool is_opened = true;
		if (ImGui::BeginDock("Navigation1", &is_opened, ImGuiWindowFlags_NoScrollWithMouse))
		{
			{
				static float f = 0.0f;
				ImGui::Text("Hello Mango");
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("clear color", (float*)&clear_color);
				if (ImGui::Button("Test Window")) show_test_window ^= 1;
				if (ImGui::Button("Another Window")) show_another_window ^= 1;
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::Text("Application elapse %.3f ms", dt);
			}

			static float color[4] = { 0, 0, 0, 0 };
			ImGui::ColorPicker(color, true);
		}
		ImGui::EndDock();
	}
	{
		static bool is_opened = true;
		if (ImGui::BeginDock("Navigation2", &is_opened, ImGuiWindowFlags_MenuBar))
		{
			{
				static float f = 0.0f;
				ImGui::Text("Hello Mango");
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("clear color", (float*)&clear_color);
				if (ImGui::Button("Test Window")) show_test_window ^= 1;
				if (ImGui::Button("Another Window")) show_another_window ^= 1;
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				static float color[4] = { 0, 0, 0, 0 };
				ImGui::ColorPicker(color, true);
			}
		}
		ImGui::EndDock();
	}
	{
		static bool is_opened = true;
		if (ImGui::BeginDock("Navigation3", &is_opened))
		{
			{
				static float f = 0.0f;
				ImGui::Text("Hello Mango");
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("clear color", (float*)&clear_color);
				if (ImGui::Button("Test Window")) show_test_window ^= 1;
				if (ImGui::Button("Another Window")) show_another_window ^= 1;
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				static float color[4] = { 0, 0, 0, 0 };
				ImGui::ColorPicker(color, true);
			}
		}
		ImGui::EndDock();
	}

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	//{
	//	static float f = 0.0f;
	//	ImGui::Text("Hello Mango");
	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color);
	//	if (ImGui::Button("Test Window")) show_test_window ^= 1;
	//	if (ImGui::Button("Another Window")) show_another_window ^= 1;
	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//}

	//// 2. Show another simple window, this time using an explicit Begin/End pair
	//if (show_another_window)
	//{
	//	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	//	ImGui::Begin("Another Window", &show_another_window);
	//	ImGui::Text("Hello");
	//	ImGui::End();
	//}

	//// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	//if (show_test_window)
	//{
	//	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
	//	ImGui::ShowTestWindow(&show_test_window);
	//}

	//ShowTabDemo();

	return true;
}

int RE::GameStudio::Loop() {
	RE::Window window;
	window.RenderHandle = [this](float dt) {
		Update(dt);
	};
	return window.loop() ? 0 : 1;
}
