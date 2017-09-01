#pragma once

#include "engine/object/GameObject.h"
#include "IEditor.h"
#include "libs/imgui/imgui.h"

void RE::GameObject::OnGUI() {
	if (ImGui::CollapsingHeader(ClassName(), ImGuiTreeNodeFlags_DefaultOpen)) {
		StaticString<128> sname = name;
		ImGui::Spacing(); ImGui::SameLine();
		if (ImGui::InputText("Name", sname.data, 128)) {
			if (sname != name) {
				name = sname.data;
			}
		}
		ImGui::Spacing(); ImGui::SameLine(); ImGui::InputText("UUID", uuid.data, 65, ImGuiInputTextFlags_ReadOnly);
	}

	for (int idx = 0; idx < _components.size(); ++idx) {
		_components[idx]->OnGUI();
	}
}