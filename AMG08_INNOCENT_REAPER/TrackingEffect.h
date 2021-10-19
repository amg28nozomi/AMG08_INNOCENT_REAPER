/*****************************************************************//**
 * @file   TrackingEffect.h
 * @brief  追跡エフェクト（エフェクトベースのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "EffectBase.h"
#include <memory>

namespace inr {
	/** 二重インクルード防止 */
	class ObjectBase;
	/** 追跡エフェクト */
	class TrackingEffect : public EffectBase {
	public:
		/**
		 * @brief						コンストラクタ
		 * @param game			ゲームクラスの参照
		 * @param gh				画像キー
		 * @param spawnpos	生成座標
		 * @param maxFrame	生存フレーム数
		 * @param direction	向き
		 */
		TrackingEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		/**
		 * @brief						デストラクタ
		 */
		~TrackingEffect() = default;
		/**
		 * @brief						更新処理
		 */
		void Process() override;
		/**
		 * @brief						追従処理の設定
		 * \param target		追従対象
		 * \param fixx			修正値(x)
		 * \param fixy			修正値(y
		 */
		void Set(ObjectBase* target, double fixx = 0, double fixy = 0);
	private:
		ObjectBase* _target;	//!< 追跡目標
		Vector2 _fix;					//!< 修正量
	};
}

