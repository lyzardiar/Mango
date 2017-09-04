#pragma once

namespace RE {
	class IEditor {
	public:
		static void DrawWindowBorad(float xpad, float ypad);
		static bool RightClickComponent(class IComponent*);
	};
}