#pragma once
#include <vector>
#include <unordered_map>
#include "Collision.h"

class Vector2;
class AABB;

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

		using Keys = std::pair<std::string, std::string>;

		double _speed;	// �ړ����x
		double _gravity;	// �d�͂ɂ������x
		bool _direction;	// �����t���O
		bool _stand;	// �n�ʂɗ����Ă��邩�ǂ���

		Vector2 _position;	// ���W
		AABB _mainCollision;	// �L�����̓����蔻��

		void ActionBox(double dx, double dy, const std::pair<int, int> wid);
	public:
		ObjectBase(Game& game);
		virtual ~ObjectBase();

		virtual void Init();		// ������
		virtual void Process();		// �X�V
		virtual void Draw();		// �`��

		inline ObjectType GetType() { return _type; }
		inline Vector2 GetPosition() { return _position; }
		inline AABB& GetMainCollision() { return _mainCollision; }

		using KeyDates = std::unordered_map<std::string, std::pair<int, int>>;

		using boxs = std::unordered_map<std::string, AABB>;
	};
}
