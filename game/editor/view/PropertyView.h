#pragma once

#include "IGUIView.h"
#include "engine/editor/EditorTransform.h"

namespace RE {
	class PropertyView : public IGUIView {
	public:
		void OnGUI() override;

	public:
		EditorTransform editorTransform;
	};
}

