#include "engine/component/Transform.h"
#include "engine/editor/IEditor.h"
#include "libs/imgui/imgui.h"

void RE::Transform::OnGUI() {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat2("Position", &x, 0.5f, 0.0f, 0.0f, "%.1f");
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat2("Scale", &sx, 0.02f, 0.0f, 0.0f, "%.2f");
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat("Rotation", &rotation, 1.0f, -360.0f, 360.0f, "%.1f");
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat2("Size", &w, 0.1f, 0.0f, 0.0f, "%.1f");
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat2("Anchor", &ax, 0.02f, 0.0f, 0.0f, "%.2f");
	}
}