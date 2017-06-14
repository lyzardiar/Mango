

#include "HierarchyView.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace RE {

	void HierarchyView::OnGUI() {
		bool is_opened = true;
		if (ImGui::BeginDock("Hierarchy", &is_opened, ImGuiWindowFlags_NoScrollWithMouse)) {
		}
		ImGui::EndDock();
	}

}
