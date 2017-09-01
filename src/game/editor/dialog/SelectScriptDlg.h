#pragma once

#include "IDialog.h"
#include "../manager/AssetsManager.h"
#include "renderer/Texture2D.h"
#include "engine/system/Texture2DSystem.h"
#include "renderer/GL/REGL.h"
#include "imgui/imgui.h"

namespace RE {
	class SelectScriptDlg : public IDialog {
	public:
		SelectScriptDlg() {
		}

		void Open() {
			ImGui::OpenPopup(PopupName);
		}

		virtual void OnGUI() {
			auto& files = AssetsManager::instance.scriptFiles;

			if (ImGui::BeginPopup(PopupName)) {
				int size = files.size;
				for (int i = 0; i < size; ++i) {
					if (ImGui::Selectable((StaticString<128>(i, ": ", files[i])).data)) {
						selectPath = files[i];
					}
				}

				ImGui::EndPopup();
			}
		}

		AssetsManager::PathType selectPath;
		const char* PopupName = "SelectScriptDlg##OpenPopup";
	};
}