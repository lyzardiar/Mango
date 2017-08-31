
#include <stdio.h>

#include "MainMenuView.h"
#include "imgui/imgui.h"


namespace RE {

	void MainMenuView::OnGUI() {
		height = 0;
		if (ImGui::BeginMainMenuBar())
		{
			onFileMenu();
			
			char status[200];
			sprintf(status, "FPS: %.1f", ImGui::GetIO().Framerate);

			auto stats_size = ImGui::CalcTextSize(status);
			ImGui::SameLine(ImGui::GetContentRegionMax().x - stats_size.x);
			ImGui::Text("%s", (const char*)status);
						
			height = ImGui::GetWindowSize().y;
			ImGui::EndMainMenuBar();
		}
	}

	void MainMenuView::onFileMenu()
	{
		if (!ImGui::BeginMenu("File")) return;

		doMenuItem("newUniverse", true);
		if (ImGui::BeginMenu("Open"))
		{

			doMenuItem("save", true);
			doMenuItem("saveAs", true);
			doMenuItem("exit", true);
			ImGui::EndMenu();
		}
		doMenuItem("save", true);
		doMenuItem("saveAs", true);
		doMenuItem("exit", true);
		ImGui::EndMenu();
	}

}
