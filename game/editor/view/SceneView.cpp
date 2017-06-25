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

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImVec2 real_size = ImGui::GetContentRegionAvail();
			ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
			ImVec2 canvas_size = real_size;
			canvas_pos.x -= 1.0f;
			canvas_pos.y -= 1.0f;

			canvas_size.x += 2.0f;
			canvas_size.y += 2.0f;

			ImColor bg(0xC0, 0xC0, 0xC0, 0x00);
			draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), bg, bg, bg, bg);
			draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(0xF3, 0x33, 0xA3));

			Engine::instance.camera.Set2D((int)real_size.x, (int)real_size.y);

			ImGui::Image((GLuint*)Engine::instance.GetTextureHandle(), real_size, ImVec2(0, 1), ImVec2(1, 0));
			
		}
		ImGui::EndDock();
	}

}
