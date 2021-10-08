#include "EffectBase.h"
#include "EffectBase.h"
#include "MapChips.h"
#include "ResourceServer.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include "EffectServer.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include <DxLib.h>

namespace inr {

	EffectBase::EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, bool direction) : _game(game), _position(spawnpos),_collision(Vector2(), Vector2()) {
		_count = 0;
		_graphKey = gh;
		_alive = maxFrame;
		_delete = false;
		_direction = direction;	
		_isDamage = false;
		_loop = 0;
		_dMax = 0;
	}
	// ������
	void EffectBase::Init() {
	}
	// �X�V
	void EffectBase::Process() {
		++_count;	// �J�E���^���Z	
		// �_���[�W���肪����ꍇ�̓_���[�W�������Ăяo��
		if(IsDamage() == true) Damage();
		// �J�E���^���������Ԃɓ��B�����ꍇ
		if (_count == (_alive - 1)) {
			// ���[�v�������Ȃ��ꍇ�͏����������s��
			if (_loop == 0) {
				_delete = true;	// �����t���O���I���ɂ���i�����\��j
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->DelOn();
				return;
			}
			--_loop;	// ���[�v���Z
			_count = 0;	// �J�E���^������
		}
	}
	// �`��
	void EffectBase::Draw() {
		// �`����W�̎Z�o
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// �O���t�B�b�N�n���h���̎擾
		int graph;
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE, _direction);
#ifdef _DEBUG
		if (_game.IsDebugMode() != true) return;
		if (_isDamage != true) return;
		// �f�o�b�O���[�h���A�_���[�W���肪����ꍇ�̂ݓ����蔻��̕`����s��
		auto db = _collision;
		auto min = db.GetMin();
		auto max = db.GetMax();
		// �`����W�̃N�����v
		_game.GetMapChips()->Clamp(min);
		_game.GetMapChips()->Clamp(max);
		DxLib::DrawBox(min.IntX(), min.IntY(), max.IntX(), max.IntY(), GetColor(255, 255, 255), FALSE);
#endif
	}
	// �A�j���[�V�����ԍ��̎Z�o
	int EffectBase::GraphNumber() {
		// ���݂̉摜�̕��������擾
		auto allnum = graph::ResourceServer::GetAllNum(_graphKey);
		auto interval = _alive / allnum;		// �P�\���Ԃ̊���o��
		auto no = _count / interval % allnum;	// �`�悷��A�j���[�V�����ԍ��̎Z�o
		return no;
	}
	// �O���t�B�b�N�n���h���̌���
	void EffectBase::GraphResearch(int* gh) {
		*gh = graph::ResourceServer::GetHandles(_graphKey, GraphNumber());
	}
	// �_���[�W����̐ݒ�
	void EffectBase::SetDamageEffect(int width, int height, int dinter) {
		// �_���[�W�������������
		_isDamage = true;	// �_���[�W�I��
		_collision = { _position, width / 2, height / 2, true };
		_dInter = dinter;
	}
	// �_���[�W����̐ݒ�
	void EffectBase::SetDamageEffect(int width1, int width2, int height1, int height2, int dinter, int max) {
		// �e��_���[�W�̐ݒ�
		_isDamage = true;
		_collision = { _position, width1, width2, height1, height2, true };
		_dInter = dinter;
		// �I��������0�ȉ��̏ꍇ�͍Ō�̃A�j���[�V�����ԍ���o�^
		if (max <= 0) _dMax = graph::ResourceServer::GetAllNum(_graphKey) - 1;
		else _dMax = max;	// �����łȂ��ꍇ�͒��o�^
	}
	// ���[�v�ݒ�
	void EffectBase::SetLoop(int max) {
		if (max <= 0) max = 0;	// ������0�ȉ��̏ꍇ�͏C��
		_loop = max;	// �o�^
	}
	// �_���[�W����
	void EffectBase::Damage() {
		// ���@
		auto player = _game.GetObjectServer()->GetPlayer();
		if (_collision.HitCheck(player->GetMainCollision()) != true) return;
		player->Damage(IsPlayerPosition());	// �ڐG���肪����ꍇ�̓_���[�W�����Ăяo��
	}
	// ���@�͍��E�ǂ���ɂ��邩
	bool EffectBase::IsPlayerPosition() {
		auto player = _game.GetObjectServer()->GetPlayer();
		// ���݂̍��W���玩�@�̍��W���������ꍇ�A���̒l�͐����ǂ���ɂȂ��Ă��邩�H
		auto pos = _position.GetX() - player->GetPosition().GetX();
		if (pos < 0) return false;	// �����ɂ���
		else return true;			// �E���ɋ���
	}
	// �_���[�W�����̔���
	bool EffectBase::IsDamage() {
		if (_isDamage != true) return false;	// �_���[�W���薳��
		// ���݂̃A�j���[�V�����ԍ��̓_���[�W���肪���邩�H
		auto no = GraphNumber();
		if (_dInter < no && no <= _dMax) return true;	// �_���[�W����L��
		return false;	// �_���[�W���薳��
	}
}