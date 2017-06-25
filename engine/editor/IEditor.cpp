#pragma once

#include "IEditor.h"
#include "libs/imgui/imgui.h"
#include "engine/object/GameObject.h"

void RE::IEditor::DrawTransformUI(GameObject* go) {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat2("Position", &go->transform.x, 0.5f, 0.0f, 0.0f, "%.1f");
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat2("Scale", &go->transform.sx, 0.02f, 0.0f, 0.0f, "%.2f");
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat("Rotation", &go->transform.rotation, 1.0f, -360.0f, 360.0f, "%.1f");
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat2("Size", &go->transform.w, 0.1f, 0.0f, 0.0f, "%.1f");
		ImGui::Spacing(); ImGui::SameLine(); ImGui::DragFloat2("Anchor", &go->transform.ax, 0.02f, 0.0f, 0.0f, "%.2f");
	}
}

void RE::IEditor::DrawTriangleRenderUI(GameObject*) {

}
