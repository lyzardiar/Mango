#pragma once

#include "IGUIView.h"
#include "imgui/imgui.h"

namespace RE {
	class SceneView : public IGUIView {
	public:
		void OnGUI() override;

	protected:
		void updateEngineInput();
		void drawGizmo();

	protected:
		ImVec2 scenePos;
		ImVec2 sceneSize;
		bool inScene = false;

		bool _startDragObject = false;
	};
}

