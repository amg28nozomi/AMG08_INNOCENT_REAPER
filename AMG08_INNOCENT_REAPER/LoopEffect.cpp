/*****************************************************************//**
 * @file   LoopEffect.cpp
 * @brief  ���[�v�G�t�F�N�g�i�G�t�F�N�g�x�[�X�̃T�u�N���X�j
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "LoopEffect.h"
#include "ObjectBase.h"
#include "Game.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "EffectServer.h"

namespace inr {

	LoopEffect::LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction) : EffectBase(game, gh, spawnpos, maxFrame, direction) {
		_owner = nullptr;	// ���L�҂̏�����
	}

	void LoopEffect::Process() {
		// �I�����肪�^�̏ꍇ�͏��������Ăяo��
		if (IsEnd() == true) _game.GetModeServer()->GetModeMain()->GetEffectServer()->DelOn();
		Move();						// �Ǐ]����
		// �A�j���[�V�����Ǘ�
		if (_count == (_alive - 1)) {
			_count = 0;			// ������
		} else ++_count;	// ���Z
	}

	bool LoopEffect::IsEnd() {
		if (_delete == true && _owner == nullptr) return false;		// ���ɏ����t���O��true�ɂȂ��Ă���
		// ���L�҂����S���Ă���ꍇ
		if (_owner->IsDead() == true) {
			_delete = true;		// �t���O�I��
			return true;			// ��������
		}
		return false;				// �������Ȃ�
	}

	void LoopEffect::SetOwner(ObjectBase* owner) {
		_owner = std::move(owner);	// �o�^
	}

	bool LoopEffect::Move() {
		if (_owner == nullptr) return false;	// ���L�҂Ȃ�
		auto movepos = _owner->GetPosition();	// ���L�҂̍��W���擾
		_position = movepos;									// ���W����
		return true;
	}
}
