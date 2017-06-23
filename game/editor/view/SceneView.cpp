#include "SceneView.h"
#include "imgui/imgui.h"
#include "engine/component/TriangleRenderer.h"
#include "core/image/PngDecoder.h"
#include "renderer/FrameBuffer.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"


namespace RE {
	void SceneView::OnGUI() {
		bool is_opened = true;
		if (ImGui::BeginDock("Scene", &is_opened, ImGuiWindowFlags_NoScrollWithMouse)) {
			bool open = true;
			ImGui::ShowTestWindow(&open);
			auto size = ImGui::GetContentRegionAvail();

			Engine::instance.camera.Set2D((int)size.x, (int)size.y);

			ImGui::Image((GLuint*)Engine::instance.GetTextureHandle(), size, ImVec2(0, 1), ImVec2(1, 0));

		}
		ImGui::EndDock();
	}

}
