#pragma once
#include <vector>
#include <unordered_map>
#include <DxLib.h>
#include "Collision.h"

class Vector2;
class AABB;

namespace {
	using KeyDatas = std::unordered_map<std::string, std::pair<int, int>>;
	using Keys = std::pair<std::string, std::string>;
}

namespace inr {

	constexpr auto FRAME_G = 0.5;	// 1�t���[�����̉����l

	class Game;
	class ObjectServer;

	class ObjectBase {
		friend class ObjectServer;
	protected:
		enum class ObjectType {
			OBJECT_BASE, PLAYER, ENEMY, SOUL
		};
		Game& _game;
		ObjectType _type;

		int _aCount;	// �A�j���[�V�����̓Y����
		double _speed;	// �ړ����x
		double _gravity;	// �d�͂ɂ������x
		bool _direction;	// �����t���O
		bool _stand;	// �n�ʂɗ����Ă��邩�ǂ���
		bool _changeGraph;	// �`��O���t�B�b�N�؂�ւ��p�t���O

		Vector2 _position;	// ���W
		AABB _mainCollision;	// �L�����̓����蔻��
		Keys _divKey;	// ����:�O���t�B�b�N�p�A�E��:�T�E���h�p
		KeyDatas _motionKey;	// first:�L�[, s.first:���t���[����, SE�̍Đ�����

		virtual bool GraphResearch(int* gh);	// ����
		virtual int GetSize(const std::string& key);
		virtual int SoundResearch(const std::string& key);
		virtual int GetSoundFrame(const std::string& key);

		void ActionBox(double dx, double dy, const std::pair<int, int> wid);
#ifdef _DEBUG
		void DrawDebugBox(Collision cb, int color = GetColor(255, 255, 0));
#endif
	public:
		ObjectBase(Game& game);
		virtual ~ObjectBase();

		virtual void Init();		// ������
		virtual void Process();		// �X�V
		virtual void Draw();		// �`��

		inline bool GetDirection() { return _direction; }
		inline ObjectType GetType() { return _type; }
		inline Vector2 GetPosition() { return _position; }
		inline AABB& GetMainCollision() { return _mainCollision; }

		using boxs = std::unordered_map<std::string, AABB>;
	};
}
