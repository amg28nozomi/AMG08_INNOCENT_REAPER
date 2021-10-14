/*****************************************************************//**
 * @file   EffectBase.h
 * @brief  エフェクトのスーパークラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>

namespace inr {
	/** エフェクト */
	namespace effect {
		// エフェクトの画像キー名
		constexpr auto JUMP = "jump_effect";						//!< 自機ジャンプエフェクト
		constexpr auto DASH = "dash_effect";						//!< 自機ダッシュエフェクト
		constexpr auto ROB = "rob_effect";							//!< 自機奪うエフェクト
		constexpr auto HIT = "hit_effect";							//!< 自機ダメージエフェクト
		constexpr auto DEAD = "dead_effect";						//!< 自機死亡エフェクト
		constexpr auto GIVE = "give_effect";						//!< 自機与えるエフェクト
		constexpr auto DEBUFF = "playe_debuff_effect";	//!< 自機デバフエフェクト
		// ジャンプエフェクトの画像サイズ
		constexpr auto JUMP_IMAGE_W = 150;							//!< 自機ジャンプエフェクトの幅
		constexpr auto JUMP_IMAGE_H = 50;								//!< 自機ジャンプエフェクトの高さ
		// 奪うエフェクトの画像サイズ
		constexpr auto ROB_IMAGE = 140;									//!< 自機奪うエフェクトのサイズ
		// ダメージエフェクトの画像サイズ
		constexpr auto HIT_IMAGE_W = 840;								//!< ダメージエフェクトの幅
		constexpr auto HIT_IMAGE_H = 430;								//!< ダメージエフェクトの高さ
		// 与えるエフェクトの画像サイズ
		constexpr auto GIVE_IMAGE = 200;								//!< 与えるエフェクトのサイズ
		// デバフエフェクトの画像サイズおよび総分割数
		constexpr auto DEBUF_IMAGE = 140;								//!< デバフエフェクトのサイズ
		constexpr auto DEBUF_MAX = 15;									//!< デバフエフェクトの総分割数
		/** 敵のコンパイル時定数 */
		namespace enemy {
			constexpr auto S_HIT = "soul_hit_effect";			//!< 死亡エフェクト
			constexpr auto SHIT_IMAGE = 400;							//!< 死亡エフェクトのサイズ
			constexpr auto HITDROP = "hipdrop_effect";		//!< ヒップドロップエフェクト
			constexpr auto HIPDROP_WIDTH = 560;						//!< ヒップドロップエフェクトの幅
			constexpr auto HIPDROP_HEIGHT = 280;					//!< ヒップドロップエフェクトの高さ
			constexpr auto HIPDROP_MAX = 24;							//!< ヒップドロップエフェクトの総分割数
		}
		/** ソルジャードール */
		namespace soldier {
			constexpr auto OMEN = "sd_omen_effect";				//!< 自機発見エフェクト
			constexpr auto OMEN_IMAGE = 160;							//!< 自機発見エフェクトのサイズ
			constexpr auto OMEN_NUMS = 14;								//!< 自機発見エフェクトの総分割数
		}
		/** ビッグドール */
		namespace bigdoll {
			constexpr auto OMEN = "bd_omen_effect";				//!< 自機発見エフェクト
			constexpr auto OMEN_IMAGE = 280;							//!< 自機発見エフェクトのサイズ
			constexpr auto OMEN_NUMS = 14;								//!< 自機発見エフェクトの総分割数
		}
	}
	/** エフェクトのスーパークラス */
	class EffectBase {
	public:
		/**
		 * @brief						コンストラクタ
		 * @param game			ゲームクラスの参照
		 * @param gh				画像キー
		 * @param spawnpos	生成座標
		 * @param maxFrame	生存フレーム数
		 * @param direction	反転フラグ
		 */
		EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		/**
		 * @brief	デストラクタ
		 */
		~EffectBase() = default;
		/**
		 * @brief	初期化処理
		 */
		virtual void Init();
		/**
		 * @brief	更新処理
		 */
		virtual void Process();
		/**
		 * @brief	描画処理
		 */
		virtual void Draw();
		// ダメージ判定の設定(引数1:横幅　引数2:縦幅　引数3:ダメージ判定が発生するまでの猶予時間)
		/**
		 * @brief					ダメージ判定の設定
		 * @param width		横幅
		 * @param height	縦幅
		 * @param dinter	ダメージ判定が発生するまでの猶予フレーム
		 */
		virtual void SetDamageEffect(int width, int height, int dinter = 0);
		/**
		 * @brief					ダメージ判定の設定
		 * @param width1	横幅(min)
		 * @param width2	横幅(max)
		 * @param height1	縦幅(min)
		 * @param height2	縦幅(max)
		 * @param dinter	ダメージ判定が発生するまでの猶予フレーム
		 * @param max			ダメージ判定の終了フレーム
		 */
		virtual void SetDamageEffect(int width1, int width2, int height1, int height2, int dinter = 0, int max = 0);
		/**
		 * @brief			ループの設定
		 * @param max	ループ回数
		 */
		void SetLoop(int max);
		/**
		 * @brief		消去フラグの取得
		 * @return	消去フラグを返す
		 */
		inline bool IsDel() { return _delete; }
	protected:
		Game& _game;						//!< ゲームクラス参照
		Vector2 _position;			//!< 座標
		AABB _collision;				//!< 当たり判定
		int _count;							//!< カウンタ
		int _alive;							//!< 生存時間
		int _allNum;						//!< 総描画枚数
		int _interval;					//!< モーションが完了するまでの猶予時間
		int _dInter;						//!< ダメージ判定が発生するまでの猶予時間
		int _dMax;							//!< ダメージ判定の終了時間
		int _loop;							//!< ループ回数（デフォルトだとなし）
		bool _direction;				//!< 生成時の向き
		bool _isDamage;					//!< ダメージ判定はあるか？
		bool _delete;						//!< 消去するかの判定
		std::string _graphKey;	//!< グラフィックハンドルのキー
		/**
		 * @brief			グラフィックハンドルの検索
		 * @param gh	グラフィックハンドル格納用のアドレス
		 */
		void GraphResearch(int* gh);
		/**
		 * @brief	自機との接触判定およびダメージ判定の呼び出し処理
		 */
		void Damage();
		/**
		 * @brief		自機が左右どちら側に居るのかの判定
		 * @return	右側に居る場合はtrueを返す
		 *					左側に居る場合はfalseを返す
		 */
		bool IsPlayerPosition();
		/**
		 * @brief		ダメージ処理を行うかの判定
		 * @return	ダメージ判定がある場合はtrueを返す
		 *					それ以外の場合はfalseを返す
		 */
		bool IsDamage();
		/**
		 * @brief		アニメーション番号の取得
		 * @return	算出したアニメーション番号を返す
		 */
		int GraphNumber();
	};
}

