#pragma once
#include "ObjectBase.h"
#include "Vector2.h"

namespace inr {

	class Soul : public ObjectBase {
	public:
		Soul(Game& game);
		~Soul() override;

		void Process() override;
		void Draw() override;
	private:
		// ���̐F�i�ԁA�j
		enum class Type {
			RED, BLUE
		};

		Type _sType;
		Vector2 _moveVector;

		void Tracking(); // �ǐՏ���
		void Move();	// �ړ�
	public:
		inline Type GetType() { return _sType; }
	};
}

