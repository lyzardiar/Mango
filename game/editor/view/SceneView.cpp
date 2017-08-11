#include "SceneView.h"
#include "imgui/imgui.h"
#include "engine/component/TriangleRenderer.h"
#include "core/image/PngDecoder.h"
#include "renderer/FrameBuffer.h"
#include "engine/Engine.h"
#include "engine/component/Camera.h"
#include "engine/editor/IEditor.h"
#include "engine/system/InputSystem.h"
#include "engine/object/GameObject.h"


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

			drawGizmo();
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
		
		Engine::instance.input.curMouseWheel = ImGui::GetIO().MouseWheel;
	}

	void SceneView::drawGizmo() {
		auto& input = Engine::instance.input;
		if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered()) {
			if (Engine::instance.selectedObjs.size > 0) {
				auto& trans = Engine::instance.selectedObjs[0]->transform;

				Vec2 localPos = trans.WorldToLocal(Vec2(input.curMousePos.x, input.curMousePos.y));
				if (Rect(0, 0, trans.w, trans.h).Contains(localPos)) {
					_startDragObject = true;
				}
			}

			if (!_startDragObject) {
				auto obj = input.PickUp(Engine::instance.root, input.curMousePos.x, input.curMousePos.y);

				if (obj) {
					Engine::instance.SelectObjects(&obj, 1);
					_startDragObject = true;
				}
				else {
					Engine::instance.SelectObjects(nullptr, 0);
				}
			}
		}

		if (Engine::instance.selectedObjs.size <= 0) return;
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		auto go = Engine::instance.selectedObjs[0];
		auto& trans = go->transform;

		Vec2 a;
		Vec2 b = a + Vec2(trans.w, 0);
		Vec2 c = a + trans.size;
		Vec2 d = a + Vec2(0, trans.h);

		a = trans.LocalToWorld(a);
		b = trans.LocalToWorld(b);
		c = trans.LocalToWorld(c);;
		d = trans.LocalToWorld(d);

		auto toImVec2 = [this](Vec2& pos) -> ImVec2 {
			ImVec2 ret = scenePos;
			ret.x += pos.x;
			ret.y += sceneSize.y - pos.y;
			return ret;
		};

		ImVec2 canvas_a = toImVec2(a);
		ImVec2 canvas_b = toImVec2(b);
		ImVec2 canvas_c = toImVec2(c);
		ImVec2 canvas_d = toImVec2(d);

		draw_list->AddQuad(canvas_a, canvas_b, canvas_c, canvas_d, ImColor(0xFF, 0x33, 0x33));

		if (ImGui::IsMouseReleased(0)) {
			_startDragObject = false;
		}

		if (_startDragObject) {
			auto& io = ImGui::GetIO();
			trans.x += io.MouseDelta.x;
			trans.y -= io.MouseDelta.y;
		}
	}

}
