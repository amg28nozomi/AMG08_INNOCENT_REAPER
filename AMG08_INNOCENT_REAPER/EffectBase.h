/*****************************************************************//**
 * \file   EffectBase.h
 * \brief  エフェクトのスーパークラス
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
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
			constexpr auto OMEN = "sd_omen_effect";		// 自機発見エフェクト
			constexpr auto OMEN_NUMS = 14;				// 総分割数
			constexpr auto OMEN_IMAGE = 160;			// 画像サイズ
		}
		// ビッグドール
		namespace bigdoll {
			constexpr auto OMEN = "bd_omen_effect";		// 自機発見エフェクト
			constexpr auto OMEN_NUMS = 14;				// 総分割数
			constexpr auto OMEN_IMAGE = 280;			// 画像サイズ
		}
	}
	// エフェクトの基底クラス
	class EffectBase {
	public:
		// コンストラクタ(引数1:Gameクラスの参照　引数2:画像キー　引数3:生成地点　引数4:生存フレーム数　引数5:反転フラグ)
		EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		// デストラクタ
		~EffectBase() = default;
		// 初期化
		virtual void Init();
		// 更新
		virtual void Process();
		// 描画
		virtual void Draw();
		// ダメージ判定の設定(引数1:横幅　引数2:縦幅　引数3:ダメージ判定が発生するまでの猶予時間)
		virtual void SetDamageEffect(int width, int height, int dinter = 0);
		// ダメージ判定の設定(引数1:横幅(min)　引数2:横幅(max)　引数3:縦幅(min)　引数4:縦幅(max)　引数5:ダメージ判定が発生するまでの猶予時間　引数6:ダメージ判定の終了時間)
		virtual void SetDamageEffect(int width1, int width2, int height1, int height2, int dinter = 0, int max = 0);
		// ループ設定(引数1:ループ回数)
		void SetLoop(int max);
		// このエフェクトを削除するか？
		inline bool IsDel() { return _delete; }
	protected:
		Game& _game;			// ゲームクラス参照
		Vector2 _position;		// 座標
		AABB _collision;		// 当たり判定
		int _count;				// カウンタ
		int _alive;				// 生存時間
		int _allNum;			// 総描画枚数
		int _interval;			// モーションが完了するまでの猶予時間
		int _dInter;			// ダメージ判定が発生するまでの猶予時間
		int _dMax;				// ダメージ判定の終了時間
		int _loop;				// ループ回数（デフォルトだとなし）
		bool _direction;		// 生成時の向き
		bool _isDamage;			// ダメージ判定はあるか？
		bool _delete;			// 消去するかの判定
		std::string _graphKey;	// グラフィックハンドルのキー
		// グラフィックハンドルの検索
		void GraphResearch(int* gh);
		// 自機との接触判定およびダメージ判定の呼び出し処理
		void Damage();
		// 自機が左右どちら側に居るのかの判定(true:右側に居る　false:左側に居る）
		bool IsPlayerPosition();
		// ダメージ処理を行うかの判定
		bool IsDamage();
		// アニメーション番号の算出
		int GraphNumber();
	};
}

