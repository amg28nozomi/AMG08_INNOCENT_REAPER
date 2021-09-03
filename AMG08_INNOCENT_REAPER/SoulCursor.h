#pragma once
#include "ObjectBase.h"
#include "RedGreenBlue.h"

namespace inr {

	namespace ui {
		constexpr auto CURSOR = "ui_cursor";
	}

	class SoulCursor : public ObjectBase {
	public:
		SoulCursor(Game& game);
		~SoulCursor() = default;

		void Init() override;
		void Process() override;		// �X�V
		void Draw() override;	// �`��
	private:
		Vector2 _moveVector;
		RedGreenBlue _rgb;
		bool _input;	// ���͉͂\���H

		bool _setBlend;	// �F�ʂ͒����������H

		void Move(int lever1, int lever2);	// ���́^�ړ�����
		void PositionUpdate();
		void ChangeBlendGraph();	// ��ނɉ������P�x�̕ύX
	};
}

