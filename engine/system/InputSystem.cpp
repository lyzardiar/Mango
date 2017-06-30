#include "InputSystem.h"
#include "core/platform/Platform.h"

namespace RE {


	void InputSystem::OnMouseDown(float x, float y) {
		Log("Mouse Begin: %.2f  %.2f", x, y);
	}

	void InputSystem::OnMouseUp(float x, float y) {
		Log("Mouse End: %.2f  %.2f", x, y);
	}

	void InputSystem::OnMouseMove(float x, float y, float dx, float dy) {
		Log("Mouse Move: %.2f  %.2f %.2f  %.2f", x, y, dx, dy);
	}

}