#include "Editor.h"

RE::Editor RE::Editor::instance;

void RE::Editor::Update(float dt) {
	welcomView.isOpen = false;

	if (welcomView.isOpen) {
		welcomView.OnGUI();
	}
	else {

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
