#pragma once
#include "imgui/imgui.h"
#include "../editor/view/WelcomeView.h"
#include "../editor/view/MainMenuView.h"
#include "../editor/view/HierarchyView.h"
#include "../editor/view/SceneView.h"
#include "../editor/view/PropertyView.h"

using namespace RE;

class Test1 {
public:
	static void Update(float dt) {
		bool show_test_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImColor(114, 144, 154);

		static WelcomeView welcomView;
		welcomView.isOpen = false;
		
		if (welcomView.isOpen) {
			welcomView.OnGUI();
		}
		else {
			static MainMenuView mainView;
			static HierarchyView hierarchyView;
			static SceneView sceneView;
			static PropertyView propertyView;

			mainView.OnGUI();

			if (ImGui::GetIO().DisplaySize.y > 0)
			{
				auto pos = ImVec2(0, mainView.height);
				auto size = ImGui::GetIO().DisplaySize;
				size.y -= pos.y;
				ImGui::RootDock(pos, size);
			}

			hierarchyView.OnGUI();
			sceneView.OnGUI();
			propertyView.OnGUI();
		}
	}
};