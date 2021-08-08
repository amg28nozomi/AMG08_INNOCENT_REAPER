#pragma once
#include "Vector2.h"

namespace inr {

	class ObjectBase;

	class Camera {
	public:
		Camera(ObjectBase& obj);
		~Camera() = default;

		void Process();
		void Draw();
	private:
		ObjectBase& _owner;		// camera�̏��L��
		Vector2 _worldPosition;	// ���[���h���W

	};
}

