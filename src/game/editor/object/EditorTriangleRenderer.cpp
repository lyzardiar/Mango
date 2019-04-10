#include "engine/component/TriangleRenderer.h"
#include "libs/imgui/imgui.h"
#include "core/platform/Platform.h"
#include "engine/component/IComponent.h"
#include "engine/object/GameObject.h"
#include "engine/system/Texture2DSystem.h"
#include "core/base/Path.h"

void RE::TriangleRenderer::OnGUI() {
	if (ImGui::CollapsingHeader("TriangleRenderer", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		Path lastPath = material.texture ? material.texture->path : "";

		material.OnGUI();

		if (lastPath != material.texture->path) {
			material.texture = Texture2DSystem::instance.Add(material.texture->path);
			gameObject->transform.size = material.texture->size;

		}

		if (gameObject->transform.w != 0 && gameObject->transform.h != 0) {
			// Todo: for Image Quad
			if (triangles.verts.size == 4) {
				auto w = gameObject->transform.w;
				auto h = gameObject->transform.h;

				triangles.verts[1].x = w;
				triangles.verts[2].y = h;
				triangles.verts[3].x = w;
				triangles.verts[3].y = h;
			}
		}
		
		ImGui::Spacing(); ImGui::SameLine();
		ImGui::ColorEdit4("Color", &color.r);
		if (ImGui::BeginPopupContextItem("color context menu##TriangleRenderer")) {
			ImGui::Text("Edit color");
			if (ImGui::ColorPicker(&color.r, true)) {
				this->setColor(color);
			}
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		ImGui::Unindent();
	}
}