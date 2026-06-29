/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "KeyboardMouseController.h"
#include "Mouse.h"
#include "Keyboard.h"

using namespace NCL;
float KeyboardMouseController::GetAxis(uint32_t axis) const {
	// XAxis  = strafe (left / right)
	// ZAxis  = forward / backward
	// YAxis  = up / down
	// XAxisMouse / YAxisMouse = mouse look (unchanged)

	if (axis == XAxis) {
		// LEFT / RIGHT arrows move camera sideways
		if (keyboard.KeyDown(NCL::KeyCodes::LEFT)) {
			return -1.0f;
		}
		if (keyboard.KeyDown(NCL::KeyCodes::RIGHT)) {
			return 1.0f;
		}
	}
	else if (axis == ZAxis) {
		// UP / DOWN arrows move camera forward / back
		if (keyboard.KeyDown(NCL::KeyCodes::UP)) {
			return 1.0f;
		}
		if (keyboard.KeyDown(NCL::KeyCodes::DOWN)) {
			return -1.0f;
		}
	}
	else if (axis == YAxis) {
		// PageUp / PageDown move camera up / down
		//if (keyboard.KeyDown(NCL::KeyCodes::I)) {      // PageUp
		//	return 1.0f;
		//}
		//if (keyboard.KeyDown(NCL::KeyCodes::K)) {       // PageDown
		//	return -1.0f;
		//}

		return 0.0f;
	}
	else if (axis == XAxisMouse) {
		return mouse.GetRelativePosition().x;
	}
	else if (axis == YAxisMouse) {
		return mouse.GetRelativePosition().y;
	}

	return 0.0f;
}


float	KeyboardMouseController::GetButtonAnalogue(uint32_t button) const {
	return GetButton(button);
}

bool	KeyboardMouseController::GetButton(uint32_t button)  const {
	if (button == LeftMouseButton) {
		return mouse.ButtonDown(NCL::MouseButtons::Left);
	}
	if (button == RightMouseButton) {
		return mouse.ButtonDown(NCL::MouseButtons::Right);
	}
	return 0.0f;
}