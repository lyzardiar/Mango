#pragma once

#include "IDialog.h"
#include "../manager/AssetsManager.h"
#include "engine/system/Texture2DSystem.h"

namespace RE {
	class SelectImageDlg : public IDialog {
	public:
		SelectImageDlg() {
		}

		void Open() {
			ImGui::OpenPopup(PopupName);
		}

		virtual void OnGUI() {
			auto& files = AssetsManager::instance.imageFiles;

			if (ImGui::BeginPopup(PopupName)) {
				int size = files.size;
				for (int i = 0; i < size; ++i) {
					ImGui::Image((GLuint*)Texture2DSystem::instance.Add(files[i])->GetHandle(), ImVec2(20, 20), ImVec2(0, 1), ImVec2(1, 0));

					ImGui::SameLine();
					if (ImGui::Selectable((StaticString<128>(i, ": ", files[i])).data)) {
						selectPath = files[i];
					}

				}

				ImGui::EndPopup();
			}
		}

		AssetsManager::PathType selectPath;
		const char* PopupName = "SelectImageDlg##OpenPopup";
	};
}

