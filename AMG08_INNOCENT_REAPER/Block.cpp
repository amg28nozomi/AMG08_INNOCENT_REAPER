#include "Block.h"
#include "Game.h"
#include "SoundServer.h"
#include "GimmickServer.h"

namespace inr {

	Block::Block(Game& game) : GimmickBase(game) {
		_gType = GimmickType::BLOCK;	// �M�~�b�N�^�C�v�̐ݒ�
		_divKey = { gimmick::block::KEY_BLOCK, gimmick::block::KEY_BLOCK };	
		_motionKey = { 
			{gimmick::block::KEY_BLOCK, {1, 0}},
			{gimmick::block::KEY_BREAK, { 26 * 2, 0}} };

		_pal = 255;
		_break = gimmick::block::BRAKE_OFF;	// ���Ă��Ȃ�
	}

	void Block::Init() {

	}

	void Block::Process() {
		// �t���O���I�����A�A�j���[�V�������I�����Ă���ꍇ�͊Y���I�u�W�F�N�g����������
		if (_break == gimmick::block::BRAKE_ON && AnimationCountMax() == true) {
			_delete = true;
			_game.GetGimmickServer()->DelOn();
			return;
		}
		if (_break != gimmick::block::BRAKE_ON) return;	// ���Ă��Ȃ��ꍇ�������𔲂���
		++_aCount;
	}

	void Block::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph;
		GraphResearch(&graph);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif
	}

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
		_break = gimmick::block::BRAKE_OFF;
		_pal = 255;
	}

	void Block::ObjValueUpdate() {
		auto flag = oscenario::gimmick::FLAG_FALSE;
		if (_break == gimmick::block::BRAKE_ON) flag = oscenario::gimmick::FLAG_TRUE;
		_oValue.FlagUpdate(flag);	// �t���O�X�V
	}


	bool Block::Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection) {
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
		}
		else if (0 < move.GetX()) {
			pos.GetPX() = _position.GetX() - _mainCollision.GetWidthMin() - box.GetWidthMin();
			move.GetPX() = 0;
		}
		return true;
	}

	bool Block::Break() {
		auto sound = SoundResearch(_divKey.second);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_divKey.second));
		_break = gimmick::block::BRAKE_ON;	// �j��t���O���I���ɂ���
		_divKey.first = gimmick::block::KEY_BREAK;	// �L�[��j��G�t�F�N�g�ɐ؂�ւ�
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = false;
#endif
		return true;
	}
}
