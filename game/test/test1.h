#pragma once
#include "imgui/imgui.h"

class Test1 {
public:
	static void Update(float dt) {
		bool show_test_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImColor(114, 144, 154);

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
	}
};