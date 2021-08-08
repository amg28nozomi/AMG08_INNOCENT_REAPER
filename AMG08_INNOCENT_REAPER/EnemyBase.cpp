#include "EnemyBase.h"
#include "Game.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"

using std::string;

namespace inr {

	namespace {
		constexpr auto ESCAPE_MAX = 1000;
		constexpr auto ESCAPE_VECTOR = 80;	// 1�b��(60frame)�ňړ����鋗��

		constexpr auto FRAME = 60;
	}


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
		auto x = _position.IntX();
		auto y = _position.IntY();

		int graph;	// �O���t�B�b�N�n���h���i�[�p
		GraphResearch(&graph);	// �n���h���擾
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);

		std::string& key = _divKey.first;
		auto box = _collisions.find(key);
		if (box != _collisions.end()) {
			if (box->second.GetDrawFlg() == true) {
				box->second.Draw(GetColor(255, 0, 0));
			}
		}
		//// �A�j���[�V�������I����Ă��Ȃ��ꍇ�̓J�E���g�𑝂₷
		//if (_aCount < GetSize(_divKey.first)) { ++_aCount; }
		//else AnimationInit();	// �J�E���^�[������
		_mainCollision.DrawBox();
		_searchBox.DrawBox();
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
			_divKey.first = enemy::SOLDIER_ESCAPE;
			// ���g�̌����ɂ���Ĉړ�������ύX
			if (_direction) _actionX = -ESCAPE_MAX / FRAME;
			else _actionX = ESCAPE_MAX / FRAME;
		}
		// �����ɂ���ĉ��Z�E���Z�؂�ւ�
		if (_direction) {
			_actionX -= ESCAPE_VECTOR / FRAME;
			_moveVector.GetPX() = ESCAPE_VECTOR / FRAME;
			return;
		}
		_actionX += ESCAPE_VECTOR / FRAME;
		_moveVector.GetPX() = ESCAPE_VECTOR / FRAME;
	}

	int EnemyBase::GetSize(const std::string& key) {
		// �v�f����
		auto it = _enemyKey.find(key);
		if (it == _enemyKey.end()) {
			// �C�e���[�^��������Ȃ����ꍇ��-1��Ԃ�
#ifdef _DEBUG
			OutputDebugString("�L�[���q�b�g���܂���ł����B�L�[�����m�F���Ă��������B\n");
#endif
			return -1;
		}
		return it->second.first;
	}

	bool EnemyBase::GraphResearch(int* gh) {
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

	int EnemyBase::SoundResearch(const std::string& key) {
		_divKey.second = key;
		auto sound = se::SoundServer::GetSound(_divKey.second);
		return sound;
	}

	int EnemyBase::GetSoundFrame(const std::string& key) {
		// �L�[����
		auto it = _enemyKey.find(key);
		// �q�b�g���Ȃ������ꍇ��-1��Ԃ��B
		if (it == _enemyKey.end()) {
			return -1;
		}
		// SE�̍Đ��t���[�����Ԃ����o���ĕԂ��B
		auto soundFrame = it->second.second;
		return soundFrame;
	}
}