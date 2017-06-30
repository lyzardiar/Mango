#pragma once

#include "IEditor.h"
#include "libs/imgui/imgui.h"
#include "engine/object/GameObject.h"

void RE::IEditor::DrawClassUI(class GameObject* go) {
	if (ImGui::CollapsingHeader(go->ClassName(), ImGuiTreeNodeFlags_DefaultOpen)) {
		StaticString<128> name;
		name = go->name;
		ImGui::Spacing(); ImGui::SameLine(); ImGui::InputText("Name", name.data, 128);
		if (name != go->name) {
			go->name = name.data;
		}

		StaticString<128> uuid = go->uuid;
		ImGui::Spacing(); ImGui::SameLine(); ImGui::InputText("UUID", uuid.data, 128);
	}
}

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
