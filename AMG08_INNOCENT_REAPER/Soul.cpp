#include "Soul.h"
#include "ObjectBase.h"
#include "Vector2.h"
#include "game.h"
#include "ObjectServer.h"

namespace inr {

	Soul::Soul(Game& game) : ObjectBase(game) {
		_sType = Type::RED;
	}

	Soul::~Soul() {

	}

	void Soul::Process() {

	}

	void Soul::Draw() {

	}

	void Soul::Tracking() {
		// �v���C���[�̎Q�Ƃ��擾
		auto player = _game.GetObjectServer()->GetPlayer();
		auto sx = _position.GetX();
		auto sy = _position.GetY();
		auto px = player.GetPosition().GetX();
		auto py = player.GetPosition().GetY();
		// �v���C���[�̎Q�Ƃ��擾
		auto player = _game.GetObjectServer()->GetPlayer();
		// ���g�ƃv���C���[�Ԃ̃x�N�g�����Z�o
		Vector2 mv = { px - sx, py - sy };
	}
}
