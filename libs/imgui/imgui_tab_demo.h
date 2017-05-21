#pragma once

#include "imgui/imgui_tabs.h"

static bool ShowTabDemo() {
	ImGui::SetNextWindowPos(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);

	ImGuiWindowFlags window_flags = 0;
	bool p_open = true;
	if (!ImGui::Begin("ImGui Tab Demo", &p_open, window_flags)) {
		ImGui::End();
		return true;
	}

	ImGui::BeginTabBar("Settings#left_tabs_bar");
	if (ImGui::AddTab("General")) {
		static bool fullscreen = false, multi = false;
		if (ImGui::Checkbox("Fullscreen Mode", &fullscreen)) {

		}
		if (ImGui::Checkbox("Enable Multisampling", &multi)) {

		}
		static int a = 1, b = 0, c = 100;
		if (ImGui::SliderInt("MSAA Count", &a, b, c)) {

		}
	}
	if (ImGui::AddTab("GUI")) {
		ImGui::Text("Tab 2");
	}
	if (ImGui::AddTab("Tab Name")) {
		ImGui::Text("Tab 3");
	}
	if (ImGui::AddTab("Tab Name")) {
		ImGui::Text("Tab 4");
	}
	ImGui::EndTabBar();

	ImGui::Dummy(ImVec2(0, 20));

	ImGui::BeginTabBar("#Additional Parameters");
	float value = 0.0f;
	if (ImGui::AddTab("Tab Name2")) {
		ImGui::SliderFloat("Slider", &value, 0, 1.0f);
	}
	if (ImGui::AddTab("Tab Name3")) {
		ImGui::Text("Tab 2");
	}
	if (ImGui::AddTab("Tab Name4")) {
		ImGui::Text("Tab 3");
	}
	if (ImGui::AddTab("Tab Name5")) {
		ImGui::Text("Tab 4");
	}
	ImGui::EndTabBar();
	ImGui::End();
}