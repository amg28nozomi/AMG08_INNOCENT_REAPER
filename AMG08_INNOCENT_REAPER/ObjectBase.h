#pragma once
#include <unordered_map>
#include <DxLib.h>
#include "Collision.h"
#include "ResourceServer.h"
#include "ObjectValue.h"

class Vector2;
class AABB;

namespace {
	using KeyDatas = std::unordered_map<std::string, std::pair<int, int>>;
	using Keys = std::pair<std::string, std::string>;
}

namespace inr {

	constexpr auto FRAME_G = 0.5;	// 1�t���[�����̉����l
	constexpr auto MAX_G = 15.0;
	constexpr auto SE_NUM = 0;

	class Game;
	class ObjectServer;

	class ObjectBase {
		friend class ObjectServer;
	protected:
		enum class ObjectType {
			OBJECT_BASE, PLAYER, ENEMY, SOUL, GIMMICK
		};
		Game& _game;
		ObjectType _type;

		int _aCount;	// �A�j���[�V�����̓Y����
		double _speed;	// �ړ����x
		double _gravity;	// �d�͂ɂ������x
		bool _direction;	// �����t���O
		bool _stand;	// �n�ʂɗ����Ă��邩�ǂ���
		bool _changeGraph;	// �`��O���t�B�b�N�؂�ւ��p�t���O
		bool _delete;	// �f���[�g���邩�ۂ��H�itrue�FObjectServer��������@false:�������j
		bool _changeDirection;	// �����̕ύX�����������ۂ�
		bool _isJump;

		ObjectValue _oValue;	// �������̏��
		Vector2 _position;	// ���W
		Vector2 _lastChip;	// �Ō�ɗ����Ă����}�b�v�`�b�v
		AABB _mainCollision;	// �L�����̓����蔻��
		Keys _divKey;	// ����:�O���t�B�b�N�p�A�E��:�T�E���h�p
		KeyDatas _motionKey;	// first:�L�[, s.first:���t���[����, SE�̍Đ�����

		virtual bool GraphResearch(int* gh);	// ResourceServer����O���t�B�b�N�n���h���擾
		virtual bool IsStandChip();	// �}�b�v�`�b�v�̏�ɗ����Ă��邩�H
		virtual int GetSize(const std::string& key);	// �A�j���[�V�����̍Đ��t���[��
		virtual void AnimationCount();	// �A�j���[�V�����J�E���^�̑���
		int AnimationInterval();	// �A�j���[�V�����̕`��Ԋu
		int AnimationNumber();	// ���݂̕`��ԍ����擾
		bool IsAnimationMax();	// �A�j���[�V�����͕`�悵�I�������H
		bool AnimationCountMax();
		

		virtual int SoundResearch(const std::string& key);
		virtual int GetSoundFrame(const std::string& key);
		virtual AABB NowCollision(std::string key);	// ���݂̓����蔻���Ԃ�
		bool ThisPlayer();	// ����͎��@��

		void ActionBox(double dx, double dy, const std::pair<int, int> wid);
		void GimmickCheck(Vector2& move);	// �M�~�b�N�ɂ߂荞��ł��邩�H
#ifdef _DEBUG
		void DrawDebugBox(Collision cb, int color = GetColor(255, 0, 255));
#endif
	public:
		ObjectBase(Game& game);
		virtual ~ObjectBase();

		virtual void Init();		// ������
		virtual void Process();		// �X�V
		virtual void Draw();		// �`��

		// virtual void CollisionHit(std::string ckey, Collision acollision, bool direction);	// �e��Փ˔���

		inline bool GetDirection() { return _direction; }
		inline bool IsDelete() { return _delete; }	// �I�u�W�F�N�g���J�����邩�H
		inline ObjectType GetType() { return _type; }
		inline Vector2 GetPosition() { return _position; }
		inline virtual Vector2 GetMoveVector() { return Vector2(); }	// �ړ��x�N�g���̃Q�b�^�[
		inline AABB& GetMainCollision() { return _mainCollision; }
		inline ObjectValue GetObjectValue() { return _oValue; }
		virtual bool IsDead();	// ����ł��邩�H

		virtual void SetParameter(ObjectValue objValue);
		virtual void SetParameter(Vector2 newpos);
		virtual void SetStatus(Vector2 spawn, std::string soulcolor) { return; } // status
		virtual void SetSpeed(double speed) { return; }
		void Del();	//�@���g�̏����t���O��ύX
		// �����܂�

		using boxs = std::unordered_map<std::string, AABB>;
	};
}
