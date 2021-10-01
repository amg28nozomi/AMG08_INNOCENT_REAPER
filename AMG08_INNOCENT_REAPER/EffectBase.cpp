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
	}


	void EffectBase::Init() {

	}

	void EffectBase::Process() {
		++_count;
		if (_isDamage == true) {
			if(IsDamage() == true) Damage();
		}
		if (_count == (_alive - 1)) {
			if (_loop == 0) {
				_delete = true;	// �����t���O���I���ɂ���i�����\��j
				_game.GetModeServer()->GetModeMain()->GetEffectServer()->DelOn();
				return;
			}
			--_loop;
			_count = 0;
		}
	}

	void EffectBase::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);	// �摜�ʒu���C������
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, TRUE, _direction);
		// �f�o�b�O���̂݁A�����蔻���`�悷��
#ifdef _DEBUG
		auto db = _collision;
		auto min = db.GetMin();
		auto max = db.GetMax();
		_game.GetMapChips()->Clamp(min);	// �摜�ʒu���C������
		_game.GetMapChips()->Clamp(max);	// �摜�ʒu���C������
		DxLib::DrawBox(min.IntX(), min.IntY(), max.IntX(), max.IntY(), GetColor(255, 255, 255), FALSE);
#endif
	}

	int EffectBase::GraphNumber() {
		auto allnum = graph::ResourceServer::GetAllNum(_graphKey);
		auto interval = _alive / allnum;	// �P�\���Ԃ̊���o��
		auto no = _count / interval % allnum;	// �`�悷��摜�̎Z�o
		return no;
	}

	void EffectBase::GraphResearch(int* gh) {
		*gh = graph::ResourceServer::GetHandles(_graphKey, GraphNumber());
	}

	void EffectBase::SetDamageEffect(int width, int height, int dinter) {
		// �_���[�W�������������
		_isDamage = true;	// �_���[�W�I��
		_collision = { _position, width / 2, height / 2, true };
		_dInter = dinter;
	}

	void EffectBase::SetDamageEffect(int width1, int width2, int height1, int height2, int dinter) {
		_isDamage = true;	// �_���[�W�I��
		_collision = { _position, width1, width2, height1, height2, true };
		_dInter = dinter;
	}

	void EffectBase::SetLoop(int max) {
		if (max < 0) max = 0;
		_loop = max;
	}

	void EffectBase::Damage() {
		// �_���[�W�������s��
		auto player = _game.GetObjectServer()->GetPlayer();
		if (_collision.HitCheck(player->GetMainCollision()) != true) return;
		player->Damage(IsPlayerPosition());	// ���@�̃_���[�W�������Ăяo��
	}

	bool EffectBase::IsPlayerPosition() {
		auto player = _game.GetObjectServer()->GetPlayer();
		auto pos = _position.GetX() - player->GetPosition().GetX();
		if (pos < 0) return false;
		else return true;
	}

	bool EffectBase::IsDamage() {
		bool isdamage = _dInter <= GraphNumber();
		return isdamage;
	}
}