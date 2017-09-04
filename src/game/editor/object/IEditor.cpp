#pragma once

#include "IEditor.h"
#include "libs/imgui/imgui.h"
#include "engine/object/GameObject.h"

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

bool RE::IEditor::RightClickComponent(IComponent* comp) {
	bool isRemoved = false;
	if (ImGui::IsItemClicked(1)) {
		ImGui::OpenPopup("RightClickComponent");
	}
	if (ImGui::BeginPopup("RightClickComponent")) {
		if (comp->canRemove && ImGui::Button("Remove Component")) {
			isRemoved = true;
			comp->gameObject->RemoveComponentImt(comp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	return isRemoved;
}
