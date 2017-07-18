#pragma once

#include "IGUIView.h"

#include "core/base/Array.h"

namespace RE {
	class HierarchyView : public IGUIView {
	public:
		void OnGUI() override;

		void showObject(class GameObject* obj, const Array<class GameObject*>& selectedObjects);

	public:
		;
	};
}

