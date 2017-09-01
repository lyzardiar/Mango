

#include "HierarchyView.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "engine/Engine.h"
#include "engine/object/GameObject.h"
#include "engine/component/TriangleRenderer.h"
#include "game/editor/object/IEditor.h"
#include "../dialog/SelectScriptDlg.h"

namespace RE {

	void HierarchyView::OnGUI() {
		bool is_opened = true;
		if (ImGui::BeginDock("Hierarchy", &is_opened, ImGuiWindowFlags_NoScrollWithMouse)) {
			auto& root = Engine::instance.root;

			IEditor::DrawWindowBorad(4.0f, 1.0f);
			showObject(root, Engine::instance.selectedObjs);
		}
		ImGui::EndDock();
	}

	void HierarchyView::showObject(GameObject* obj, const Array<class GameObject*>& selectedObjects) {
		auto& name = obj->name + PtrString(obj).data;
		bool selected = selectedObjects.Find(obj) >= 0;

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (selected ? ImGuiTreeNodeFlags_Selected : 0);
		if (obj->children.size > 0) {
			bool node_open = ImGui::TreeNodeBehavior(window->GetID(PtrString(obj).data), node_flags, name.c_str(), NULL);
			checkItemRightClick(obj);
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
				Engine::instance.SelectObjects(&obj, 1);
			
			if (node_open) {
				for (int idx = 0; idx < obj->children.size; ++idx) {
					showObject(obj->children[idx], selectedObjects);
				}
				ImGui::TreePop();
			}
		}
		else {
			bool node_open = ImGui::TreeNodeBehavior(window->GetID(PtrString(obj).data), node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, name.c_str(), NULL);
			checkItemRightClick(obj);
			if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
				Engine::instance.SelectObjects(&obj, 1);			
		}
	}

	void HierarchyView::checkItemRightClick(class GameObject* obj){
		if (ImGui::IsItemClicked(1)) {
			puts(obj->name.c_str());
			ImGui::OpenPopup(PtrString(obj).data);
		}
		if (ImGui::BeginPopup(PtrString(obj).data)) {
			if (ImGui::Button("Create GameObject")) {
				obj->AddChild(new GameObject("GameObject"));
				ImGui::CloseCurrentPopup();
			}
			if (obj->transform.parent && ImGui::Button("Remove GameObject")) {
				auto parent = obj->parent;
				parent->RemoveChild(obj);
				ImGui::CloseCurrentPopup();
			}


			showAddComponentPopup(obj);
			ImGui::EndPopup();
		}
	}

	RE::Texture2D* testTexture() {
		static RE::Texture2D* tex = nullptr;
		tex = tex ? tex : new RE::Texture2D("images/1.png");
		return tex;
	}

	void HierarchyView::showAddComponentPopup(class GameObject* obj) {
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("Add Component##Popup");
		}
		if (ImGui::BeginPopup("Add Component##Popup")) {
			if (ImGui::Button("Add TriangleRender")) {
				auto renderer = obj->AddComponent<TriangleRenderer>();
				ImGui::CloseCurrentPopup();
			}

			SelectScriptDlg dlg;
			if (ImGui::Button("Add Script Component")) {
				dlg.Open();

			}
			dlg.OnGUI();
			if (!dlg.selectPath.Empty()) {
				obj->AddScriptComponent(dlg.selectPath.data);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

}
