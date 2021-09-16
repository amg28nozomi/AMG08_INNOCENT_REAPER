#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "MapChips.h"

#include "SoulSkin.h"

using std::string;

namespace {
	constexpr auto FRAME = 30;
}

namespace inr {


	EnemyBase::EnemyBase(Game& game) : ObjectBase(game), _searchBox(Vector2(), Vector2()) {
		_eType = EnemyType::SOLDIER_DOLL;
		_aState = ActionState::EMPTY;
		_divKey = std::make_pair("NULL", key::SOUND_NUM);
		_position = { 0, 0 };
		_moveVector = { 0, 0 };
		_actionX = 0;
		_patrolX = 0;
		_aInterval = 0;

		_aFrame = 0;
		_aCount = 0;
		_sounds = 0;
		_stay = 0;
		_direction = false;
		_changeGraph = true;
		_drawStop = false;

		_soul = nullptr;

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

		/*std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				DrawDebugBox(box->second, GetColor(255, 0, 0));
			}
		}*/
		//// �A�j���[�V�������I����Ă��Ȃ��ꍇ�̓J�E���g�𑝂₷
		//if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		//else AnimationInit();	// �J�E���^�[������
		/*DrawDebugBox(_mainCollision);*/
		DrawDebugBox(_searchBox);

		/*auto it = _collisions.find(_divKey.first);
		if (it == _collisions.end()) return;
		DrawDebugBox(it->second);*/
	}

	void EnemyBase::AnimationCount() {
		if (_soul == nullptr && IsAnimationMax() == true) return;
		ObjectBase::AnimationCount();
	}

	/*void EnemyBase::AnimationInit() {

	}

	void EnemyBase::Move() {

	}

	void EnemyBase::Action() {

	}*/

	bool EnemyBase::SearchPlayer() {
		// ������ł͂Ȃ���
		if (_soul != nullptr) {
			// �v���C���[�N���X�̎Q�Ƃ��烁�C���̓����蔻����擾
			auto player = _game.GetObjectServer()->GetPlayer();
			const auto& pbox = player->GetMainCollision();
			// ���G�͈͂Ǝ��@�͓������Ă��邩�H
			if (_searchBox.HitCheck(pbox)) {
				// ���@�͎����Ɠ��������ɂ��邩�H
				if (_direction == SearchPosition()) {
					// ����
					return true;
				}
			}
		}
		// �����蔻�肪��������
		return false;
	}

	bool EnemyBase::SearchPosition() {
		auto pl = _game.GetObjectServer()->GetPlayer();
		// �v���C���[��x���W���擾
		auto px = pl->GetPosition().GetX();
		// Vector 
		auto myx = _position.GetX();
		// Enemy��x���W����player��x���W������
		// ���̒l��+�̏ꍇ�̓v���C���[��x���W���G����������(�����ɂ���)
		// ���̋t�Œl��-�̏ꍇ�͉E���ɂ���
		auto way = myx - px;
		// �l�����̐��̏ꍇ�i�E���ɂ���j
		if (way < 0) return false;
		// �l�����̐��̏ꍇ�i�����ɂ���j
		return true;
	}

	void EnemyBase::PositionUpdate() {

	}

	void EnemyBase::EscapeOn() {
	}

	void EnemyBase::PatrolOn() {
		_searchBox.GetCollisionFlgB() = true;
		_changeGraph = true;
		_aState = ActionState::PATROL;
	}

	void EnemyBase::AttackOn() {

	}

	void EnemyBase::ChangeState(ActionState nextstate, std::string key) {
		_changeGraph = true;
		_aState = nextstate;
		_divKey.first = key;
	}

	void EnemyBase::ChangeIdol() {

	}

	bool EnemyBase::Hit() {
		return false;
	}

	void EnemyBase::SetParameter(ObjectValue objValue) {
		_oValue = objValue;
		_position = _oValue.Positions()[0];	// ���W�X�V
		_mainCollision.Update(_position, _direction);	// �����蔻��C��
		_searchBox.Update(_position, _direction);	// ���G�͈͂̏C��
	}

	void EnemyBase::CollisionHit(const std::string ckey, Collision acollision, bool direction) {

	}

	bool EnemyBase::IsEmpty() {
		if (_soul == nullptr) return true;
		return false;
	}

	void EnemyBase::Death() {
		_soul->SetSpwan(_position);	// ���g�̍��W�ɍ������̉�����
		_soul->OwnerNull();
		_soul.reset();	// ���̏��L���������
	}

	void EnemyBase::Action() {
		// �v���C���[�𔭌��ł��邩
		if (SearchPlayer() == true) {
			// �����Ă��鍰�ɉ����ď�����ύX����
			switch (_soul->SoulColor()) {
				// �Ԃ����̎��́A�ːi���������s����B
			case soul::RED:
				AttackOn();
				if (_actionX == 0) {
					PatrolOn();
				}
				break;
			case soul::BLUE:
				EscapeOn();
				if (_actionX == 0) {
					PatrolOn();
				}
				break;
			}
		}
		// �����ł��Ȃ������ꍇ�͈ړ��������s��
		if (_soul == nullptr) _actionX = 0;
	}

	AABB EnemyBase::VitalPart(Collision& col, int vital) {
		// ���W���Z�o�iy���W�͕ύX�Ȃ��j
		Vector2 vitalMin(0.0, col.GetMin().GetY());
		Vector2 vitalMax(0.0, col.GetMax().GetY());
		if (_direction) {
			vitalMin.GetPX() = col.GetMax().GetX() - vital;
			vitalMax.GetPX() = col.GetMax().GetX();
		}
		else {
			// �E�Ɍ����Ă���ꍇ
			vitalMin.GetPX() = col.GetMin().GetX();
			vitalMax.GetPX() = col.GetMin().GetX() + vital;
		}
		return AABB(vitalMin, vitalMax, true);
	}

	AABB EnemyBase::DamageBox(int fix) {
		// �x�N�g���쐬
		auto damageMin(_mainCollision.GetMin());
		auto damageMax(_mainCollision.GetMax());
		if (_direction) {
			damageMax.GetPX() -= fix;
		}
		else {
			damageMin.GetPX() += fix;
		}
		return AABB(damageMin, damageMax, true);
	}

	bool EnemyBase::IsStandChip() {
		auto nowcol = NowCollision(_divKey.first);
		auto chipType = _game.GetMapChips()->IsStand(nowcol, _position, _gravity, &_lastChip);
		switch (chipType) {
		case mapchip::IVY:
		case mapchip::NONE:
			return false;
		case mapchip::THORM:
			if (_soul != nullptr) Death();
		default:
			return true;
		}
	}
}