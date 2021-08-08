#pragma once
#include <vector>
#include <unordered_map>
#include "Collision.h"

class Vector2;
class AABB;

namespace inr {

	constexpr auto FRAME_G = 0.5;	// 1フレーム事の加速値

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

		double _speed;	// 移動速度
		double _gravity;	// 重力による加速度
		bool _direction;	// 向きフラグ
		bool _stand;	// 地面に立っているかどうか

		Vector2 _position;	// 座標
		AABB _mainCollision;	// キャラの当たり判定

		void ActionBox(double dx, double dy, const std::pair<int, int> wid);
	public:
		ObjectBase(Game& game);
		virtual ~ObjectBase();

		virtual void Init();		// 初期化
		virtual void Process();		// 更新
		virtual void Draw();		// 描画

		inline ObjectType GetType() { return _type; }
		inline Vector2 GetPosition() { return _position; }
		inline AABB& GetMainCollision() { return _mainCollision; }

		using KeyDates = std::unordered_map<std::string, std::pair<int, int>>;

		using boxs = std::unordered_map<std::string, AABB>;
	};
}
