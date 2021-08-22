#pragma once
#include <unordered_map>
#include <DxLib.h>
#include "Collision.h"
#include "ResourceServer.h"

class Vector2;
class AABB;

namespace {
	using KeyDatas = std::unordered_map<std::string, std::pair<int, int>>;
	using Keys = std::pair<std::string, std::string>;
}

namespace inr {

	constexpr auto FRAME_G = 0.5;	// 1フレーム事の加速値
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

		int _aCount;	// アニメーションの添え字
		double _speed;	// 移動速度
		double _gravity;	// 重力による加速度
		bool _direction;	// 向きフラグ
		bool _stand;	// 地面に立っているかどうか
		bool _changeGraph;	// 描画グラフィック切り替え用フラグ

		Vector2 _position;	// 座標
		AABB _mainCollision;	// キャラの当たり判定
		Keys _divKey;	// 左辺:グラフィック用、右辺:サウンド用
		KeyDatas _motionKey;	// first:キー, s.first:総フレーム数, SEの再生時間

		virtual bool GraphResearch(int* gh);	// ResourceServerからグラフィックハンドル取得
		virtual int GetSize(const std::string& key);	// アニメーションの再生フレーム
		virtual void AnimationCount();	// アニメーションカウンタの増減
		int AnimationInterval();	// アニメーションの描画間隔
		int AnimationNumber();	// 現在の描画番号を取得
		bool IsAnimationMax();	// アニメーションは描画し終えたか？
		bool AnimationCountMax();

		virtual int SoundResearch(const std::string& key);
		virtual int GetSoundFrame(const std::string& key);

		void ActionBox(double dx, double dy, const std::pair<int, int> wid);
#ifdef _DEBUG
		void DrawDebugBox(Collision cb, int color = GetColor(255, 0, 255));
#endif
	public:
		ObjectBase(Game& game);
		virtual ~ObjectBase();

		virtual void Init();		// 初期化
		virtual void Process();		// 更新
		virtual void Draw();		// 描画

		virtual void CollisionHit(std::string ckey, Collision acollision, bool direction);	// 各種衝突判定

		inline bool GetDirection() { return _direction; }
		inline ObjectType GetType() { return _type; }
		inline Vector2 GetPosition() { return _position; }
		inline virtual Vector2 GetMoveVector() { return Vector2(); }	// 移動ベクトルのゲッター
		inline AABB& GetMainCollision() { return _mainCollision; }

		// α版用
		virtual bool IsEmpty() { return false; }
		virtual void SetStatus(Vector2 spawn, std::string soulcolor) { return; } // status
		virtual void SetSpeed(double speed) { return; }
		// ここまで

		using boxs = std::unordered_map<std::string, AABB>;
	};
}
