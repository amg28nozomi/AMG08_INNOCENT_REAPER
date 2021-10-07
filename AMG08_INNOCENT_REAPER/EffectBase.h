#pragma once
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>

namespace inr {
	namespace effect {
		// エフェクトのキー名
		// 自機エフェクト
		constexpr auto JUMP = "jump_effect";			// ジャンプ
		constexpr auto DASH = "dash_effect";			// ダッシュ
		constexpr auto ROB = "rob_effect";				// 奪う
		constexpr auto HIT = "hit_effect";				// ダメージ
		constexpr auto DEAD = "dead_effect";			// 死亡
		constexpr auto GIVE = "give_effect";			// 与える
		constexpr auto DEBUFF = "playe_debuff_effect";	// デバフ
		// ジャンプエフェクトの画像サイズ
		constexpr auto JUMP_IMAGE_W = 150;
		constexpr auto JUMP_IMAGE_H = 50;
		// 奪うエフェクトの画像サイズ
		constexpr auto ROB_IMAGE = 140;
		// ダメージエフェクトの画像サイズ
		constexpr auto HIT_IMAGE_W = 840;
		constexpr auto HIT_IMAGE_H = 430;
		// 与えるエフェクトの画像サイズ
		constexpr auto GIVE_IMAGE = 200;
		// デバフエフェクトの画像サイズおよび総分割数
		constexpr auto DEBUF_IMAGE = 140;
		constexpr auto DEBUF_MAX = 15;
		// 敵エフェクト
		namespace enemy {
			constexpr auto S_HIT = "soul_hit_effect";	// 死亡
			constexpr auto SHIT_IMAGE = 400;			// 死亡エフェクトの画像サイズ
			constexpr auto HITDROP = "hipdrop_effect";	// ヒップドロップ(衝撃波)
			constexpr auto HIPDROP_WIDTH = 560;			// ヒップドロップの横幅
			constexpr auto HIPDROP_HEIGHT = 280;		// ヒップドロップの立幅
			constexpr auto HIPDROP_MAX = 24;			// ヒップドロップの総分割数
		}
		// ソルジャードール
		namespace soldier {
			constexpr auto OMEN = "sd_omen_effect";
			constexpr auto OMEN_NUMS = 14;
			constexpr auto OMEN_IMAGE = 160;
		}

		namespace bigdoll {
			constexpr auto OMEN = "bd_omen_effect";
			constexpr auto OMEN_NUMS = 14;
			constexpr auto OMEN_IMAGE = 280;
		}
	}

	class Game;

	// エフェクト
	class EffectBase {
	public:
		EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);	// 生成座標および、生存フレーム数、分割数、一画像当たりの待ち時間
		~EffectBase() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();	// 描画

		virtual void SetDamageEffect(int width, int height, int dinter = 0);	// エフェクトにダメージ判定を持たせる
		virtual void SetDamageEffect(int width1, int width2, int height1, int height2, int dinter = 0, int max = 0);
		void SetLoop(int max);	// 引数1 何回繰り返すか
		inline bool IsDel() { return _delete; }	// このエフェクトを削除するか？

	protected:

		Game& _game;	// ゲーム参照

		Vector2 _position;	// 座標
		AABB _collision;	// 当たり判定
		int _count;	// カウンタ
		int _alive;	// 生存時間
		int _allNum;	// 総描画枚数
		int _interval;	// モーションが完了するまでの猶予時間]
		int _dInter;	//	ダメージ判定が発生するまでの猶予時間
		int _dMax;
		int _loop;	// ループ回数（デフォルトだとなし）
		bool _direction;	// 生成時の向き
		bool _isDamage;	// ダメージ判定はあるか？
		bool _delete;	// 消去するかの判定
		std::string _graphKey;	// グラフィックハンドルのキー

		void GraphResearch(int* gh);	// ResourceServerからグラフィックハンドル取得
		void Damage();	// ダメージ処理（自機へ）
		bool IsPlayerPosition();
		bool IsDamage();
		int GraphNumber();
	};
}

