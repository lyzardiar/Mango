
#include "core/platform/Platform.h"
#include "renderer/Material.h"
#include "libs/imgui/imgui.h"
#include "game/editor/dialog/SelectImageDlg.h"


void RE::Shader::OnGUI() {

}

void RE::Texture2D::OnGUI() {
	ImGui::Spacing(); ImGui::SameLine(); ImGui::Image((GLuint*)_handle, ImVec2(60.0f, 60.0f), ImVec2(0, 1), ImVec2(1, 0));

	int len = 128;
	StaticString<128> lastPath = path;

	ImVec2 canvas_size = ImGui::GetContentRegionAvail();
	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
	ImGui::Spacing(); ImGui::SameLine();

	ImGui::PushItemWidth(canvas_size.x*0.68f);
	if (ImGui::InputText("", lastPath.data, len,
		ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue)) {
		Log("==> %s", lastPath.data);
	}
	ImGui::PopItemWidth();

	ImGui::SameLine(0.0f, 10.0f);
	SelectImageDlg dlg;
	if (ImGui::Button("...")) {
		dlg.Open();
	}
	dlg.OnGUI();

	if (!dlg.selectPath.Empty()) {
		lastPath = dlg.selectPath.data;
	}

	if (lastPath != path) {
		InitWithFile(lastPath.data);
	}		
}

void RE::Material::OnGUI() {
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (texture == nullptr) texture = new Texture2D();
		if (shader == nullptr) shader = new Shader();
		texture->OnGUI();
		shader->OnGUI();
	}
}