#include "PropertyView.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "renderer/PipeLine.h"
#include "engine/Engine.h"
#include "engine/object/GameObject.h"
#include "engine/editor/IEditor.h"

namespace RE {

	void PropertyView::OnGUI() {

		bool is_opened = true;
		if (ImGui::BeginDock("Property", &is_opened, ImGuiWindowFlags_NoScrollWithMouse)) {
			IEditor::DrawWindowBorad(4.0f, 1.0f);

			Engine::instance.root->OnGUI();
			//Engine::instance.root->children[0]->OnGUI();

			//static float values[90] = { 0 };
			//static int values_offset = 0;
			//values[values_offset] = 1000.0f / ImGui::GetIO().Framerate;// cosf(phase);
			//values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);

			//char status[200];
			//sprintf(status, "FPS: %.0f/%.1f", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
			//ImGui::PlotLines("##FPS", values, IM_ARRAYSIZE(values), values_offset, status, -10.0f, 10.0f, ImVec2(0, 80));

			//if (ImGui::Button("Compile Shader")) {
			//	auto pl = PipeLine();
			//	pl.Init("shaders/default.vert", "shaders/default.frag");
			//}

			//auto ce = ImGui::BeginCurveEditor("Curve");
			//static ImVec2 p[4];
			//ImGui::CurveSegment(p, ce);
			//ImGui::EndCurveEditor(ce);
		}
		ImGui::EndDock();
	}

}
