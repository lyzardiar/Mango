#pragma once
#include "IEditor.h"
#include "libs/imgui/imgui.h"
#include "engine/object/GameObject.h"

namespace RE {
	class EditorTransform : public IEditor {
	public:
		virtual void OnGUI(class GameObject* go) override {
			if (ImGui::CollapsingHeader("Transform", &isCollapse)) {
				ImGui::DragFloat2("Position", &go->transform.x, 0.5f, 0.0f, 0.0f, "%.1f");
				ImGui::DragFloat2("Scale", &go->transform.sx, 0.02f, 0.0f, 0.0f, "%.2f");
				ImGui::DragFloat("Rotation", &go->transform.rotation, 0.1f, 0.0f, 0.0f, "%.1f");
				ImGui::DragFloat2("Size", &go->transform.w, 0.1f, 0.0f, 0.0f, "%.1f");
				ImGui::DragFloat2("Anchor", &go->transform.ax, 0.02f, 0.0f, 0.0f, "%.2f");

			}
		}

	public:
		bool isCollapse = true;
	};
}