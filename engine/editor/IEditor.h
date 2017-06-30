#pragma once

namespace RE {
	class IEditor {
	public:
		static void DrawClassUI(class GameObject*);
		static void DrawTransformUI(class GameObject*);
		static void DrawTriangleRenderUI(class GameObject*);
		static void DrawWindowBorad(float xpad, float ypad);
	};
}