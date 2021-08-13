#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "MapChips.h"

using std::string;

namespace {
	constexpr auto FRAME = 30;
}

namespace inr {


	EnemyBase::EnemyBase(Game& game) : ObjectBase(game), _searchBox(Vector2(), Vector2()) {
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::EMPTY;
		_sState = SoulState::EMPTY;
		_divKey = std::make_pair("NULL", key::SOUND_NUM);
		_position = { 0, 0 };
		_moveVector = { 0, 0 };
		_actionX = 0;

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_direction = true;
		_changeGraph = true;
		Init();
	}

	EnemyBase::~EnemyBase() {

	}

	void EnemyBase::Init() {
		// _collisions = { {enemy::ENEMY_EMPTY, {Vector2(), Vector2()}} };
	}

	void EnemyBase::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}
		//// �A�j���[�V�������I����Ă��Ȃ��ꍇ�̓J�E���g�𑝂₷
		//if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		//else AnimationInit();	// �J�E���^�[������
		DrawDebugBox(_mainCollision);
		DrawDebugBox(_searchBox);
	}

	/*void EnemyBase::AnimationInit() {

	}

	void EnemyBase::Move() {

	}

	void EnemyBase::Action() {

	}*/

	bool EnemyBase::SearchPlayer() {
		// ������ł͂Ȃ���
		if (_sState != SoulState::EMPTY) {
			// �v���C���[�N���X�̎Q�Ƃ��烁�C���̓����蔻����擾
			auto player = _game.GetObjectServer()->GetObjectA(ObjectType::PLAYER);
			const auto& pbox = player.GetMainCollision();
			// ���G�͈͂Ǝ��@�͓������Ă��邩�H
			if (_searchBox.HitCheck(pbox)) {
				// ���@�͎����Ɠ��������ɂ��邩�H
				if (_direction == SearchPosition()) {
					// ����
					return true;
				}
			}
		}
		return false;
	}

	bool EnemyBase::SearchPosition() {
		auto pl = _game.GetObjectServer()->GetObjectA(ObjectType::PLAYER);
		// �v���C���[��x���W���擾
		auto px = pl.GetPosition().GetX();
		// Vector 
		auto myx = _position.GetX();
		// Enemy��x���W����player��x���W������
		// ���̒l��+�̏ꍇ�̓v���C���[��x���W���G����������(�����ɂ���)
		// ���̋t�Œl��-�̏ꍇ�͉E���ɂ���
		auto way = myx - px;
		// �l�����̐��̏ꍇ�i�E���ɂ���j
		if (way < 0) return true;
		// �l�����̐��̏ꍇ�i�����ɂ���j
		return false;
	}

	void EnemyBase::Escape() {
		// ��Ԃ������ł͂Ȃ��ꍇ
		if (_aState != ActionState::ESCAPE) {
			_changeGraph = true;
			_aState = ActionState::ESCAPE;
			_divKey.first = enemy::blue::SOLDIER_ESCAPE;
			// ���g�̌����ɂ���Ĉړ�������ύX
			if (_direction) _actionX = -enemy::ESCAPE_MAX / FRAME;
			else _actionX = enemy::ESCAPE_MAX / FRAME;
		}
		//// �����ɂ���ĉ��Z�E���Z�؂�ւ�
		//if (_direction) {
		//	_actionX -= ESCAPE_VECTOR / FRAME;
		//	_moveVector.GetPX() = ESCAPE_VECTOR / FRAME;
		//	return;
		//}
		//_actionX += ESCAPE_VECTOR / FRAME;
		//_moveVector.GetPX() = ESCAPE_VECTOR / FRAME;
	}

	bool EnemyBase::Hit() {
		return false;
	}
}