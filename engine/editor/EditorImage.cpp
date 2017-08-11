#pragma once

#include "IEditor.h"
#include "libs/imgui/imgui.h"
#include "engine/object/Image.h"
#include "core/base/Types.h"

void RE::Image::OnGUI() {
	GameObject::OnGUI();
	
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::ColorEdit4("Color", &color.r);
	if (ImGui::BeginPopupContextItem("color context menu##TriangleRenderer")) {
		ImGui::Text("Edit color");
		ImGui::ColorPicker(&color.r, true);
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}