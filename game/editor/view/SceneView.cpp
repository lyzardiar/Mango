#include "SceneView.h"
#include "imgui/imgui.h"
#include "engine/component/TriangleRenderer.h"
#include "core/image/PngDecoder.h"
#include "renderer/FrameBuffer.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "engine/editor/IEditor.h"
#include "engine/system/InputSystem.h"


namespace RE {
	void SceneView::OnGUI() {
		bool is_opened = true;
		if (ImGui::BeginDock("Scene", &is_opened, ImGuiWindowFlags_NoScrollWithMouse)) {
			bool open = true;
			ImGui::ShowTestWindow(&open);

			IEditor::DrawWindowBorad(4.0f, 1.0f);

			scenePos = ImGui::GetCursorScreenPos();
			sceneSize = ImGui::GetContentRegionAvail();
			Engine::instance.camera.Set2D((int)sceneSize.x, (int)sceneSize.y);

			ImGui::Image((GLuint*)Engine::instance.GetTextureHandle(), sceneSize, ImVec2(0, 1), ImVec2(1, 0));
			
			inScene = ImGui::IsItemHovered();

			updateEngineInput();
		}
		ImGui::EndDock();
	}

	void SceneView::updateEngineInput() {
		auto& io = ImGui::GetIO();

		auto rel_mp = ImGui::GetMousePos();
		rel_mp.x -= scenePos.x;
		rel_mp.y -= scenePos.y;
		rel_mp.y = sceneSize.y - rel_mp.y;
		for (int i = 0; i < 3; ++i) {
			if (ImGui::IsMouseReleased(i)) {
				Engine::instance.input.OnMouseUp(rel_mp.x, rel_mp.y);
			}
			else if (ImGui::IsMouseClicked(i)) {
				Engine::instance.input.OnMouseDown(rel_mp.x, rel_mp.y);
			}
			else if (ImGui::IsMouseDown(i) && (io.MouseDelta.x != 0 || io.MouseDelta.y != 0)) {
				Engine::instance.input.OnMouseMove(rel_mp.x, rel_mp.y, io.MouseDelta.x, io.MouseDelta.y);
			}
		}

	}

	void SceneView::drawGizmo() 	{

	}

}
