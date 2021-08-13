#include "ObjectBase.h"
#include "Collision.h"
#include "Vector2.h"
#include "Game.h"
#include "MapChips.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include <DxLib.h>
#include <memory>

namespace {
	constexpr auto SPEED_NULL = 0;
}

namespace inr {

	ObjectBase::ObjectBase(Game& game) : _game(game), _mainCollision(Vector2(), Vector2() ) {
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
		_gravity += FRAME_G;	// �����x�����Z
		// �}�b�v�`�b�v�̏�ɗ����Ă��邩�ǂ���
		if (_game.GetMapChips()->IsHit(_mainCollision, _gravity)) {
			// �����x��0�̎����������Ă���
			if (0 < _gravity) {
				_stand = true;
			}
			_gravity = 0;
		} else {
			_stand = false;
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

	bool ObjectBase::GraphResearch(int* gh) {
		// �t���O���I���̎��A�`�悷��O���t�B�b�N��؂�ւ���
		if (_changeGraph) {
			_changeGraph = false;
			*gh = graph::ResourceServer::GetHandles(_divKey.first, 0);	// �ŏ��̗v�f���擾
			return true;
		}
		// �O���t�B�b�N���؂�ւ��P�\�t���[�����Z�o
		auto interval = GetSize(_divKey.first) / graph::ResourceServer::GetAllNum(_divKey.first);
		// ���Ԗڂ̃A�j���[�V�������Ăяo����Ă��邩
		auto no = _aCount / interval % graph::ResourceServer::GetAllNum(_divKey.first);
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
		auto drawFlg = cb.GetbDrawFlg();
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

	void ObjectBase::CollisionHit(std::string ckey, Collision acollision) {

	}
}