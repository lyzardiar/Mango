#pragma once

namespace RE {
	class InputSystem {
	public:
		void OnMouseDown(float x, float y);
		void OnMouseUp(float x, float y);
		void OnMouseMove(float x, float y, float dx, float dy);

	public:
		bool i = false;
	};
}