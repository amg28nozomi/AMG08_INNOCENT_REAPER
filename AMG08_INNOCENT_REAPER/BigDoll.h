/*****************************************************************//**
 * \file   BigDoll.h
 * \brief  ビッグドールクラス（エネミーベースのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "EnemyBase.h"

namespace inr {
	namespace enemy {
		// ビッグドールの画像サイズ
		constexpr auto BIG_IMAGE = 280;
		// 当たり判定
		constexpr auto BIG_WIDTH = 160;
		constexpr auto BIG_HEIGHT = 200;
		// SE検索用キー（SoundServer）
		namespace bigdoll {
			constexpr auto SE_DOWN = "big_down";	// ダウンSE
			constexpr auto SE_ESCAPE_VOICE = "big_escape";	// 逃走時ボイス
			constexpr auto SE_TACKLE = "big_tackle";	// タックルSE
			constexpr auto SE_TACKLE_VOICE = "big_tackle_voice";	// タックル時ボイス
		}
	}
	// ビッグドール(敵)
	class BigDoll : public EnemyBase {
	public:
		BigDoll(Game& game);
		~BigDoll();
		// 初期化
		void Init() override;
		// 更新
		void Process() override;
		// オブジェクト情報の登録(引数:オブジェクト情報)
		void SetParameter(ObjectValue objValue);
		// 自機アクションとの衝突判定
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	private:
		double _atkVec;	// 移動量
		int _moveCount;	// 移動カウント
		// ヒップドロップ
		void HipDrop();
		// 状態に応じた処理の更新
		void StateUpdate();
		// 巡回処理判定オン
		void PatrolOn() override;
		// 攻撃処理判定オン
		void AttackOn() override;
		// 逃走処理判定オン
		void EscapeOn() override;
		// アクション状態に移行するか
		void Action() override;
		// 死亡処理
		void Death() override;
		// 待機状態の設定
		void ChangeIdol(int stay) override;
		// 移動処理
		void Move();
		// 座標更新
		void PositionUpdate() override;
		// 攻撃処理
		void Attack();
		// 現在の当たり判定の取得
		AABB NowCollision(std::string key) override;
	};
}

