#pragma once
#include "base/Types.h"

namespace RE {
	class InputSystem {
	public:
		void OnMouseDown(float x, float y);
		void OnMouseUp(float x, float y);
		void OnMouseMove(float x, float y, float dx, float dy);


		class GameObject* PickUp(class GameObject* root, float x, float y);
	public:
		bool i = false;
		Vec2 curMousePos;
	};
}