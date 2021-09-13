#include "EffectBase.h"
#include "MapChips.h"
#include "ResourceServer.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include <DxLib.h>

namespace inr {

	EffectBase::EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame) : _game(game), _position(spawnpos), _collision(Vector2(), Vector2()) {
		_count = 0;
		_graphKey = gh;
		_alive = maxFrame;
		_delete = false;
		_isDamage = false;
	}


	void EffectBase::Init() {

	}

	void EffectBase::Process() {
		if (_count == 0) {
			_delete = true;	// �����t���O���I���ɂ���i�����\��j
			return;
		}
		++_count;
		if (_isDamage == true) Damage();
	}

	void EffectBase::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);	// �摜�ʒu���C������
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
	}

	void EffectBase::GraphResearch(int* gh) {
		auto interval = _alive / graph::ResourceServer::GetAllNum(_graphKey);	// �P�\���Ԃ̊���o��
		auto no = _count / interval % graph::ResourceServer::GetAllNum(_graphKey);	// �`�悷��摜�̎Z�o
		*gh = graph::ResourceServer::GetHandles(_graphKey, no);

		// �t���O���I���̎��A�`�悷��O���t�B�b�N��؂�ւ���
	//	if (_changeGraph) {
	//		_changeGraph = false;
	//		_aCount = 0;
	//		*gh = graph::ResourceServer::GetHandles(_divKey.first, 0);	// �ŏ��̗v�f���擾
	//		return true;
	//	}
	//	auto interval = AnimationInterval();
	//	// ���Ԗڂ̃A�j���[�V�������Ăяo����Ă��邩
	//	auto no = AnimationNumber();
	//	// �O���t�B�b�N�n���h����ǂݍ���
	//	*gh = graph::ResourceServer::GetHandles(_divKey.first, no);
	//}
	}

	void EffectBase::SetDamageEffect(int width, int height) {
		// �_���[�W�������������
		_isDamage = true;	// �_���[�W�I��
		_collision = { _position, width / 2, height / 2, true };
	}

	void EffectBase::Damage() {
		// �_���[�W�������s��
		auto player = _game.GetObjectServer()->GetPlayer();
		if (_collision.HitCheck(player->GetMainCollision()) != false) return;
		player->Damage(IsPlayerPosition());	// ���@�̃_���[�W�������Ăяo��
	}

	bool EffectBase::IsPlayerPosition() {
		auto player = _game.GetObjectServer()->GetPlayer();
		auto pos = _position.GetX() - player->GetPosition().GetX();
		if (pos < 0) return false;
		else return true;
	}
}