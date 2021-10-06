#include "ObjectBase.h"
#include "Collision.h"
#include "Vector2.h"
#include "Game.h"
#include "MapChips.h"
#include "EnemyBase.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "GimmickServer.h"

#include "GimmickBase.h"
#include "Door.h"
#include "Block.h"

#include <DxLib.h>
#include <memory>

namespace {
	constexpr auto SPEED_NULL = 0;

	constexpr auto TEST_VALUE = 15;
}

namespace inr {

	ObjectBase::ObjectBase(Game& game) : _game(game), _mainCollision(Vector2(), Vector2()), _oValue() {
		_delete = false;
		_changeDirection = false;
		_isJump = false;
		_lastChip = { 0, 0 };
		Init();
	}

	ObjectBase::~ObjectBase() {

	}

	void ObjectBase::Init() {
		_type = ObjectType::OBJECT_BASE;
		_speed = SPEED_NULL;
		_aCount = 0;
		_gravity = 1;
		_direction = false;
		_stand = false;
		_changeGraph = true;

		_position = { 0, 0 };
	}

	void ObjectBase::Process() {
		if (_changeDirection == true) _changeDirection = false;

		_gravity += FRAME_G;	// �����x�����Z
		if (MAX_G < _gravity) _gravity = MAX_G;

		auto nowcol = NowCollision(_divKey.first);

		// �}�b�v�`�b�v�̏�ɗ����Ă��邩�ǂ���
		// if (_game.GetMapChips()->IsHit(_mainCollision, _gravity)) {
		if (IsStandChip()) {
			// �����x��0�̎����������Ă���
			if (0 < _gravity) {
				_stand = true;
			}
			_gravity = 0;
		} else {
			_stand = false;
		}

		auto&& objs = _game.GetObjectServer()->GetEnemys();
		for (auto&& obj : objs) {
			if (obj->GetType() != ObjectType::ENEMY) continue;
			if (obj->IsEmpty() != true) continue;
			// �����k�̓����蔻����擾
			auto emptyBox = obj->EmptyBox();
			if (_mainCollision.HitCheck(emptyBox) == true);
				// x���W�͔͈͓��Ɏ��܂��Ă��邩
				if ((emptyBox.GetMin().GetX() < nowcol.GetMin().GetX() && nowcol.GetMin().GetX() < emptyBox.GetMax().GetX()) ||
					(emptyBox.GetMin().GetX() < nowcol.GetMax().GetX() && nowcol.GetMax().GetX() < emptyBox.GetMax().GetX())) {

					if (nowcol.GetMax().GetY() <= emptyBox.GetMin().GetY() + TEST_VALUE && emptyBox.GetMin().GetY() <= nowcol.GetMax().GetY()) {
						_stand = true;
						_isJump = false;
						_gravity = 0;

						auto h = nowcol.GetHeightMax();
						_position.GetPY() = emptyBox.GetMin().GetY() - h;
					}

				}
		}

		if (nowcol.GetMin().GetY() < 0) {
			_gravity = 0;
			_position.GetPY() = nowcol.GetHeightMin();
		}
	}

	void ObjectBase::Draw() {

	}

	int ObjectBase::GetSize(const std::string& key) {
		// �v�f����
		auto it = _motionKey.find(key);
		if (it == _motionKey.end()) {
			// �C�e���[�^��������Ȃ����ꍇ��-1��Ԃ�
#ifdef _DEBUG
			OutputDebugString("�L�[���q�b�g���܂���ł����B�L�[�����m�F���Ă��������B\n");
#endif
			return -1;
		}
		return it->second.first;
	}

	void ObjectBase::AnimationCount() {
		// �J�E���^�͑��Đ����Ԃ������������H
		if (_aCount < GetSize(_divKey.first)) { 
			// �A�j���[�V�������I����Ă��Ȃ��ꍇ�̓J�E���g�𑝂₷
			++_aCount; 
			return;
		} else { 
			// �J�E���^�[������
			_aCount = 0; 
			return;
		}
	}

	int ObjectBase::AnimationInterval() { 
		return GetSize(_divKey.first) / graph::ResourceServer::GetAllNum(_divKey.first); 
	}

	int ObjectBase::AnimationNumber() {
		// �O���t�B�b�N���؂�ւ��P�\�t���[�����Z�o
		auto interval = AnimationInterval();
		// ���Ԗڂ̃A�j���[�V�������Ăяo����Ă��邩
		return _aCount / interval % graph::ResourceServer::GetAllNum(_divKey.first);
	}

	bool ObjectBase::IsAnimationMax() {
		// ���݂̕`��͍Ōォ�ǂ����H
		bool isMax = AnimationNumber() == graph::ResourceServer::GetAllNum(_divKey.first) - 1;
		return isMax;
	}

	bool ObjectBase::AnimationCountMax() {
		if (_aCount == (GetSize(_divKey.first) - 1)) return true;
		return false;
	}

	bool ObjectBase::GraphResearch(int* gh) {
		// �t���O���I���̎��A�`�悷��O���t�B�b�N��؂�ւ���
		if (_changeGraph) {
			_changeGraph = false;
			_aCount = 0;
			*gh = graph::ResourceServer::GetHandles(_divKey.first, 0);	// �ŏ��̗v�f���擾
			return true;
		}
		auto interval = AnimationInterval();
		// ���Ԗڂ̃A�j���[�V�������Ăяo����Ă��邩
		auto no = AnimationNumber();
		// �O���t�B�b�N�n���h����ǂݍ���
		*gh = graph::ResourceServer::GetHandles(_divKey.first, no);
		return false;
	}

	int ObjectBase::SoundResearch(const std::string& key) {
		_divKey.second = key;
		auto sound = se::SoundServer::GetSound(_divKey.second);
		return sound;
	}

	int ObjectBase::GetSoundFrame(const std::string& key) {
		// �L�[����
		auto it = _motionKey.find(key);
		// �q�b�g���Ȃ������ꍇ��-1��Ԃ��B
		if (it == _motionKey.end()) {
			return -1;
		}
		// SE�̍Đ��t���[�����Ԃ����o���ĕԂ��B
		auto soundFrame = it->second.second;
		return soundFrame;
	}


#ifdef _DEBUG
	void ObjectBase::DrawDebugBox(Collision cb, int color) {
		if (_game.IsDebugMode() != true) return;	// �t���O���I���̎��̂ݕ`�揈�������s����
		auto drawFlg = cb.SetDrawFlag();
		// �`�攻��̓I���ɂȂ��Ă��邩�ǂ����H
		if (drawFlg) {
			auto min = cb.GetMin();
			auto max = cb.GetMax();
			_game.GetMapChips()->Clamp(min);
			_game.GetMapChips()->Clamp(max);

			auto minx = min.IntX();
			auto maxx = max.IntX();
			auto miny = min.IntY();
			auto maxy = max.IntY();
			DxLib::DrawBox(minx, miny, maxx, maxy, color, FALSE);
		}
	}
#endif

	void ObjectBase::ActionBox(double dx, double dy, const std::pair<int, int> wid) {
		Vector2 abmin(dx - wid.first / 2, dy + wid.second / 2);
		Vector2 abmax(dx + wid.first / 2, dy - wid.second / 2);
		// AABB newBox(abmin, abmax);
		auto red = GetColor(255, 255, 0);

		DxLib::DrawBox(abmin.IntX(), abmin.IntY(), abmax.IntX(), abmax.IntY(), red, FALSE);
	}

	void ObjectBase::GimmickCheck(Vector2& move) {
		auto gimmicks = _game.GetGimmickServer()->GetGimmicks();
		for (auto g : gimmicks) {
			// �����o�����������邩�H
			if (g->GimmickType() == gimmick::DOOR) {
				auto door = std::dynamic_pointer_cast<Door>(g);
				if(door->IsSwitch() == gimmick::OFF) door->Extrude(NowCollision(_divKey.first), _position, move, _direction, _changeDirection);
			}
			else if (g->GimmickType() == gimmick::BLOCK) {
				auto block = std::dynamic_pointer_cast<Block>(g);
				if (block->IsBreak() == gimmick::block::BRAKE_OFF) block->Extrude(NowCollision(_divKey.first), _position, move, _direction, _changeDirection);
			}
		}
	}

	bool ObjectBase::IsStandChip() {
		auto nowcol = NowCollision(_divKey.first);
		auto chipType = _game.GetMapChips()->IsStand(nowcol, _position, _gravity, &_lastChip, ThisPlayer());
		switch (chipType) {
		case mapchip::IVY:
		case mapchip::NONE:
			return false;
		default:
			return true;
		}
	}

	bool ObjectBase::ThisPlayer() {
		switch (_type) {
		case ObjectType::PLAYER:
			return true;
		default:
			return false;
		}
	}

	AABB ObjectBase::NowCollision(std::string key) {
		return AABB(Vector2(), Vector2());
	}

	void ObjectBase::Del() {
		_delete = true;
		if (_game.GetObjectServer()->DelFlag() != true) _game.GetObjectServer()->DelOn();
	}

	void ObjectBase::SetParameter(ObjectValue objValue) {

	}

	void ObjectBase::SetParameter(Vector2 newpos) {

	}

	bool ObjectBase::IsDead() {
		return false;
	}
}