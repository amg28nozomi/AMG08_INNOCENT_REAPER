/*****************************************************************//**
 * @file   Block.cpp
 * @brief  �����N���X�i�M�~�b�N�x�[�X�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Block.h"
#include "Game.h"
#include "SoundServer.h"
#include "GimmickServer.h"

namespace inr {
	// �R���X�g���N�^
	Block::Block(Game& game) : GimmickBase(game) {
		// �e�평����
		_gType = GimmickType::BLOCK;
		_divKey = { gimmick::block::KEY_BLOCK, gimmick::block::KEY_BLOCK };	
		_motionKey = { 
			{gimmick::block::KEY_BLOCK, {1, 0}},
			{gimmick::block::KEY_BREAK, { 26 * 2, 0}} };

		_pal = 255;
		_break = gimmick::block::BRAKE_OFF;
	}
	// �X�V
	void Block::Process() {
		// �t���O���I�����A�A�j���[�V�������I�����Ă���ꍇ�͊Y���I�u�W�F�N�g����������
		if (_break == gimmick::block::BRAKE_ON && AnimationCountMax() == true) {
			// �����t���O���I���ɂ���
			_delete = true;
			_game.GetGimmickServer()->DelOn();
			return;
		}
		if (_break != gimmick::block::BRAKE_ON) return;	// ���Ă��Ȃ��ꍇ�������𔲂���
		++_aCount;
	}
	// �`��
	void Block::Draw() {
		// �`����W�̎Z�o
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// �O���t�B�b�N�n���h���̏K��
		int graph;
		GraphResearch(&graph);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif
	}
	// �I�u�W�F�N�g���̓o�^
	void Block::SetParameter(ObjectValue objValue) {
		_oValue = objValue;	// �I�u�W�F�N�g���o�^
		_position = _oValue.Positions()[0];	// ���W�̐ݒ�
		_mainCollision = { _position, 45, 45, 100, 140, true };	// �����蔻��̓o�^
		// �t���O���I���̏ꍇ
		if (_oValue.GimmickFlag() == oscenario::gimmick::FLAG_TRUE) {
			_break = gimmick::block::BRAKE_ON;	// �j��ς�
			_pal = 0;
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = false;
#endif
			return;
		}
		// �j�󖳂�
		_break = gimmick::block::BRAKE_OFF;
		_pal = 255;
	}
	// �I�u�W�F�N�g���̍X�V
	void Block::ObjValueUpdate() {
		// �j�󂳂�Ă��邩�ǂ����Ńt���O��؂�ւ���
		auto flag = oscenario::gimmick::FLAG_FALSE;
		if (_break == gimmick::block::BRAKE_ON) flag = oscenario::gimmick::FLAG_TRUE;
		_oValue.FlagUpdate(flag);	// �t���O�X�V
	}
	// �����o������
	bool Block::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction) {
		// �Ώۂ̔�����W�X�V
		auto newpos = pos + move;
		box.Update(newpos, direction);
		// �Ώۂ͐ڐG���Ă��邩�H
		if (_mainCollision.HitCheck(box) == false) return false;	// �Փ˂��Ă��Ȃ�
		// �Փ˂��Ă���ꍇ�͂ǂ��瑤����߂荞��ł��邩���Z�o
		// �Ή����������։����o���������s��
		if (move.GetX() < 0) {
			pos.GetPX() = _position.GetX() + _mainCollision.GetWidthMax() + box.GetWidthMax();
			move.GetPX() = 0;
		} else if (0 < move.GetX()) {
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;	// �Փ˂��Ă���
	}
	// �j�󏈗�
	void Block::Break() {
		auto sound = SoundResearch(_divKey.second);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
		_break = gimmick::block::BRAKE_ON;			// �j��t���O���I���ɂ���
		_divKey.first = gimmick::block::KEY_BREAK;	// �L�[��j��G�t�F�N�g�ɐ؂�ւ�
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = false;
#endif
	}
}
