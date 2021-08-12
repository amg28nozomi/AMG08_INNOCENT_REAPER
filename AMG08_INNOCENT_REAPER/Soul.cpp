#include "Soul.h"
#include "ObjectBase.h"
#include "Vector2.h"
#include "game.h"
#include "ObjectServer.h"

namespace inr {

	Soul::Soul(Game& game) : ObjectBase(game), _moveVector() {
		_sType = Type::RED;
	}

	Soul::~Soul() {

	}

	void Soul::Process() {
		Tracking();
		Move();
	}

	void Soul::Draw() {

	}

	void Soul::Tracking() {
		// �v���C���[�̎Q�Ƃ��擾
		auto player = _game.GetObjectServer()->GetPlayer();
		auto px = player.GetPosition().GetX();
		auto py = player.GetPosition().GetY();
		// ���g�ƃv���C���[�Ԃ̃x�N�g�����Z�o
		Vector2 mv = { px - _position.GetX(), py - _position.GetY() };
		mv.Normalize();	// �x�N�g���̐��K��
		// �ړ��x�N�g���ɉ��Z�i�j
		_moveVector.GetPX() = mv.GetX();
		_moveVector.GetPY() = mv.GetY();
	}

	void Soul::Move() {
		_position + _moveVector;	// ���W�X�V
	}
}
