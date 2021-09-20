#pragma once
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>

namespace inr {

	namespace effect {
		constexpr auto NO_COUNT = 0;
		constexpr auto NO_MAXFRAME = 0;

		// キー名
		constexpr auto JUMP = "jump_effect";	// ジャンプエフェクトのキー
		constexpr auto DASH = "dash_effect";
		constexpr auto ROB = "rob_effect";
		constexpr auto HIT = "hit_effect";	// 自機ダメージ時
		constexpr auto DEAD = "dead_effect";	// ダメージ時エフェクト

		// 画像サイズ
		constexpr auto JUMP_IMAGE_W = 80;
		constexpr auto JUMP_IMAGE_H = 50;
		constexpr auto ROB_IMAGE = 140;	// 奪うエフェクトの画像サイズ
		constexpr auto HIT_IMAGE_W = 840;
		constexpr auto HIT_IMAGE_H = 430;
	}

	class Game;

	// エフェクト
	class EffectBase {
	public:
		EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);	// 生成座標および、生存フレーム数、分割数、一画像当たりの待ち時間
		~EffectBase() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();	// 

		virtual void SetDamageEffect(int width, int height);	// エフェクトにダメージ判定を持たせる
		inline bool IsDel() { return _delete; }	// このエフェクトを削除するか？

	protected:

		Game& _game;	// ゲーム参照

		Vector2 _position;	// 座標
		AABB _collision;	// 当たり判定
		int _count;	// カウンタ
		int _alive;	// 生存時間
		int _allNum;	// 総描画枚数
		int _interval;	// モーションが完了するまでの猶予時間]
		bool _direction;	// 生成時の向き
		bool _isDamage;	// ダメージ判定はあるか？
		bool _delete;	// 消去するかの判定
		std::string _graphKey;	// グラフィックハンドルのキー

		void GraphResearch(int* gh);	// ResourceServerからグラフィックハンドル取得
		void Damage();	// ダメージ処理（自機へ）
		bool IsPlayerPosition();
	};
}

