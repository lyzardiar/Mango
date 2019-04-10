
#include "core/platform/Platform.h"
#include "renderer/Material.h"
#include "libs/imgui/imgui.h"
#include "game/editor/dialog/SelectImageDlg.h"
#include "engine/system/GLProgramSystem.h"


void RE::GLProgram::OnGUI() {

}

void RE::Texture2D::OnGUI() {
	ImGui::Image((GLuint*)_handle, ImVec2(60.0f, 60.0f), ImVec2(0, 1), ImVec2(1, 0));

	int len = 128;
	Path lastPath = path;

	ImVec2 canvas_size = ImGui::GetContentRegionAvail();
	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();

	ImGui::PushItemWidth(canvas_size.x*0.68f);
	if (ImGui::InputText("", lastPath.data.Buff(), len,
		ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue)) {
		Log("==> %s", lastPath.data.c_str());
	}
	ImGui::PopItemWidth();

	ImGui::SameLine(0.0f, 10.0f);
	SelectImageDlg dlg;
	if (ImGui::Button("...")) {
		dlg.Open();
	}
	dlg.OnGUI();

	if (!dlg.selectPath.Empty()) {
		lastPath = dlg.selectPath;
	}

	if (lastPath != path) {
		path = lastPath;
	}		
}

void RE::Material::OnGUI() {
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (texture == nullptr) texture = new Texture2D();
		if (program == nullptr) program = GLProgramSystem::instance["Default"];
		ImGui::Indent();
		texture->OnGUI();
		program->OnGUI();
		ImGui::Unindent();
	}
}