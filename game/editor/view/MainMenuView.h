#pragma once

#include "IGUIView.h"

namespace RE {
	class MainMenuView : public IGUIView {
	public:
		void OnGUI() override;

		void onFileMenu();

		float height = 0;
	};
}

