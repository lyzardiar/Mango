#include "WelcomeView.h"
#include "imgui/imgui.h"

namespace RE {

	void WelcomeView::OnGUI() {
		auto& imIO = ImGui::GetIO();

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
		float w = imIO.DisplaySize.x, h = imIO.DisplaySize.y;

		ImVec2 size((float)w, (float)h);
		if (ImGui::Begin("Welcome", nullptr, size, -1, flags))
		{
			ImGui::Text("Welcome Mango");

			ImVec2 half_size = ImGui::GetContentRegionAvail();
			//half_size.x = half_size.x * 0.5f - ImGui::GetStyle().FramePadding.x;
			//half_size.y *= 0.75f;
			auto right_pos = ImGui::GetCursorPos();
			right_pos.x += half_size.x + ImGui::GetStyle().FramePadding.x;
			if (ImGui::BeginChild("left", half_size, true))
			{
				if (ImGui::Button("New Game")) isOpen = false;

				ImGui::Separator();
				ImGui::Text("Open Game:");
				ImGui::Indent();
				/*for (auto& univ : m_universes)
				{
					if (ImGui::MenuItem(univ.data))
					{
						m_editor->loadUniverse(univ.data);
						setTitle(univ.data);
						m_is_welcome_screen_opened = false;
					}
				}*/
				ImGui::Unindent();
			}
			ImGui::EndChild();

			ImGui::SetCursorPos(right_pos);
		}
		ImGui::End();
	}

}
