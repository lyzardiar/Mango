#pragma once

#include "IEditor.h"
#include "libs/imgui/imgui.h"
#include "engine/object/GameObject.h"

void RE::IEditor::DrawClassUI(class GameObject* go) {
	if (ImGui::CollapsingHeader(go->ClassName(), ImGuiTreeNodeFlags_DefaultOpen)) {
		StaticString<128> name = go->name;
		ImGui::Spacing(); ImGui::SameLine(); 
		if (ImGui::InputText("Name", name.data, 128)) {
			if (name != go->name) {
				go->name = name.data;
			}
		}

		StaticString<65> uuid = go->uuid;
		ImGui::Spacing(); ImGui::SameLine(); ImGui::InputText("UUID", uuid.data, 65, ImGuiInputTextFlags_ReadOnly);
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

void RE::IEditor::DrawWindowBorad(float xpad, float ypad) {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 real_size = ImGui::GetContentRegionAvail();
	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImVec2 canvas_size = real_size;

	canvas_pos.x -= xpad;
	canvas_pos.y -= ypad;

	canvas_size.x += xpad + xpad;
	canvas_size.y += ypad + ypad;

	ImColor bg(0xC0, 0xC0, 0xC0, 0x00);
	draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), bg, bg, bg, bg);
	draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(0xF3, 0x33, 0xA3));
}
