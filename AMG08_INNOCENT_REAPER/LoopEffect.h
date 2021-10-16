/*****************************************************************//**
 * @file   LoopEffect.h
 * @brief  ループエフェクト（エフェクトベースのサブクラス）
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
	/** エフェクト(無限ループ処理有り) */
	class LoopEffect : public EffectBase {
	public:
		/**
		 * @brief						コンストラクタ
		 * @param game			ゲームクラスの参照
		 * @param gh				画像キー
		 * @param spawnpos	生成地点
		 * @param maxFrame	生存フレーム数
		 * @param direction	反転フラグ
		 */
		LoopEffect(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		/**
		 * @brief						デストラクタ
		 */
		~LoopEffect() = default;
		/**
		 * @brief						更新処理
		 */
		void Process() override;
		/**
		 * @brief						所有者の設定
		 * @param						ObjectBaseのポインタ
		 */
		void SetOwner(ObjectBase* owner);
	private:
		ObjectBase* _owner;		//!< 所有者
		/**
		 * @brief		オブジェクトを削除するかの判定
		 * @return	消去する場合はtrueを返す
		 *					消去しない場合はfalseを返す
		 */
		bool IsEnd();
		/**
		 * @brief		追従処理
		 * @return	処理に成功した場合はtrueを返す
		 *					処理に失敗した場合はfalseを返す
		 */
		bool Move();
	};
}

