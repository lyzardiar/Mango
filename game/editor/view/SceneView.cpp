#include "SceneView.h"
#include "imgui/imgui.h"
#include "renderer/TriangleRenderer.h"


namespace RE {

	void SceneView::OnGUI() {
		bool is_opened = true;
		if (ImGui::BeginDock("Scene", &is_opened, ImGuiWindowFlags_NoScrollWithMouse)) {
			bool open = true;
			ImGui::ShowTestWindow(&open);

			static TriangleRenderer* r = new TriangleRenderer();
			r->draw();
		}
		ImGui::EndDock();
	}

}
