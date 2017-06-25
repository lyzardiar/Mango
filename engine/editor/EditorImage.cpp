#pragma once

#include "IEditor.h"
#include "libs/imgui/imgui.h"
#include "engine/object/Image.h"
#include "core/base/Types.h"

void RE::Image::OnGUI() {
	GameObject::OnGUI();

	if (ImGui::CollapsingHeader("TriangleRenderer", ImGuiTreeNodeFlags_DefaultOpen)) {
		GLuint texHandle = renderer->material.texture != nullptr ? renderer->material.texture->GetHandle() : 0;
		ImGui::Image((GLuint*)texHandle, ImVec2(60.0f, 60.0f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine(65.0f);

		char* path = nullptr;
		int len = 0;
		StaticString<256> lastPath;
		if (renderer->material.texture != nullptr) {
			path = renderer->material.texture->path.data;
			len = 120;

			lastPath = path;
		} else {
			static char c[1] = {0};
			path = c;
		}

		if (ImGui::InputText("", path, len,
			ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue)) {
			Log("==> %s", path);
		}

		ImVec2 canvas_size = ImGui::GetContentRegionAvail();
		ImVec2 canvas_pos = ImGui::GetCursorScreenPos();

		ImGui::SameLine(0.0f, 10.0f);
		if (ImGui::Button("...")) {

		}
		
		if (renderer->material.texture != nullptr) {
			if (lastPath != path)
				renderer->material.texture->InitWithFile(path);
		}

		ImGui::ColorEdit4("Color", &color.r);
	}
}