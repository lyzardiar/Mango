#pragma once
#include "base/Types.h"

namespace RE {
	class InputSystem {
	public:
		void OnMouseDown(float x, float y, int idx = 0);
		void OnMouseUp(float x, float y, int idx = 0);
		void OnMouseMove(float x, float y, float dx, float dy, int idx = 0);
		
		class GameObject* PickUp(class GameObject* root, float x, float y);
	public:
		bool i = false;
		Vec2 curMousePos;
		Vec2 curMouseDelta;
		float curMouseWheel;

		bool mouseState[3] = { 0 };
	};
}