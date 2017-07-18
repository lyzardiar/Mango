

#include "HierarchyView.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "engine/Engine.h"
#include "engine/object/GameObject.h"

namespace RE {

	void HierarchyView::OnGUI() {
		bool is_opened = true;
		if (ImGui::BeginDock("Hierarchy", &is_opened, ImGuiWindowFlags_NoScrollWithMouse)) {
			auto& root = Engine::instance.root;

			showObject(root, Engine::instance.selectedObjs);
		}
		ImGui::EndDock();
	}

	void HierarchyView::showObject(GameObject* obj, const Array<class GameObject*>& selectedObjects) {
		auto& name = obj->name + PtrString(obj).data;
		bool selected = selectedObjects.Find(obj) >= 0;
		if (ImGui::Selectable(name.c_str(), &selected)) {
			Engine::instance.SelectObjects(&obj, 1);
		}

		ImGui::Indent();
		for (int idx = 0; idx < obj->children.size; ++idx) {
			showObject(obj->children[idx], selectedObjects);
		}
		ImGui::Unindent();
	}

}
