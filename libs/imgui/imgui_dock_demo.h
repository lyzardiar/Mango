#pragma once

#include "imgui/ImGuiDock.h"

static ImGuiDock::Dockspace& SetupDockDemo() {
	ImGui::SetNextWindowPos(ImVec2(500, 200), ImGuiSetCond_FirstUseEver);

	ImGuiDock::Dock dock1;
	dock1.initialize("Dock1", true, ImVec2(), [](ImVec2 area) {
		ImGui::Text("Hello :)");
	});

	ImGuiDock::Dock dock2;
	dock2.initialize("Dock2", true, ImVec2(250, 300), [](ImVec2 area) {
		ImGui::Text("Hello From Dock2 :)");
	});

	static ImGuiDock::Dockspace dockspace;
	dockspace.dock(&dock1, ImGuiDock::DockSlot::None, 0, true);
	dockspace.dock(&dock2, ImGuiDock::DockSlot::Right, 250, true);

	return dockspace;
}