#include "Camera.h"
#include "Game.h"
#include "ObjectBase.h"
#include "Vector2.h"

namespace inr {

	Camera::Camera(ObjectBase& obj) : _owner(obj) {
		// íÜêSç¿ïW
		_worldPosition = { WINDOW_W / 2, WINDOW_H };
	}

	void Camera::Process() {

	}

	void Camera::Draw() {

	}
}