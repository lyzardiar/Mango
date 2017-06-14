

#include "IGUIView.h"
#include "imgui/imgui.h"

void IGUIView::doMenuItem(const char* label, bool enabled)
{
	if (ImGui::MenuItem(label, "", false, enabled))
	{

	}
}