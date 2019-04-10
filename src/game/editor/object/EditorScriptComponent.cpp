#include "engine/component/ScriptComponent.h"
#include "imgui/imgui.h"
#include "core/base/String.h"
#include "script/lua/kaguya.hpp"
#include "IEditor.h"

namespace RE {

	void ScriptComponent::OnGUI() {

		if (ImGui::CollapsingHeader(StaticString<128>("Script: ", baseName.data).data, ImGuiTreeNodeFlags_DefaultOpen)) {
			if (IEditor::RightClickComponent(this)) {
				return;
			}
			if (!isValid) {
				ImGui::Text("Invalid Component!");
				return;
			}

			ImGui::Indent(10);
			auto& keys = Class.keys<std::string>();
			std::sort(keys.begin(), keys.end());
			for (auto& k : keys) {
				if (!isupper(k[0])) continue;
				
				switch (Class[k].type())
				{
				case LUA_TSTRING: {
					const char* val = Class[k];
					StaticString<128> buff = val;
					if (ImGui::InputText(StaticString<128>(k.c_str(), "##", baseName.data).Title().data, buff.data, 128)) {
						Class[k] = (const char*)(buff.data);
					}
					break;
				}
				case LUA_TBOOLEAN: {
					ImVec2 canvas_size = ImGui::GetContentRegionAvail();
					ImGui::PushItemWidth(canvas_size.x * 0.68f);
					bool val = Class[k];
					if (ImGui::Checkbox(StaticString<128>(k.c_str(), "##", baseName.data).Title().data, &val)) {
						Class[k] = val;
					}
					ImGui::PopItemWidth();
					break;
				}
				case LUA_TNUMBER: {
					float val = Class[k];
					if (ImGui::DragFloat(StaticString<128>(k.c_str(), "##", baseName.data).Title().data, &val)) {
						Class[k] = val;
					}
					break;
				}
				case LUA_TNIL: {
					break;
				}
				default:
					break;
				}
			}
			ImGui::Unindent(10);
		}	
	}

}