#pragma once

#include "imgui/imgui.h"
#include "game/editor/view/WelcomeView.h"
#include "game/editor/view/MainMenuView.h"
#include "game/editor/view/HierarchyView.h"
#include "game/editor/view/SceneView.h"
#include "game/editor/view/PropertyView.h"

namespace RE {
	class Editor {
	public: static Editor instance;
	public:
		void Update(float dt);
	public:
		WelcomeView welcomView;
		MainMenuView mainView;
		HierarchyView hierarchyView;
		SceneView sceneView;
		PropertyView propertyView;
	};
}