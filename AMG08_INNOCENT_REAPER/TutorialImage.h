/*****************************************************************//**
 * @file   TutorialImage.h
 * @brief  チュートリアル画像（パーティクル画像のサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Particle_Image.h"
#include "AABB.h"

namespace inr {
	/** チュートリアルテキストのコンパイル時定数 */
	namespace tutorial {
		constexpr auto ROB = "titorial_rob";									//!< 奪う
		constexpr auto GIVE = "titorial_give";								//!< 与える
		constexpr auto DASH = "titorial_dash";								//!< ダッシュ
		constexpr auto JUMP = "titorial_jump";								//!< ジャンプ
		constexpr auto GRAB = "titorial_grab";								//!< 掴み
		constexpr auto INTERACTION = "titorial_interaction";	//!< 調べる
		constexpr auto CRYSTAL = "titorial_crystal";					//!< 水晶
		constexpr auto LEVER = "titorial_lever";							//!< レバー
		constexpr auto EMPTY = "titorial_empty";							//!< 抜け殻
	}
	/** 二重インクルード防止 */
	class TutorialValue;
	/** チュートリアル画像 */
	class TutorialImage : public Particle_Image {
	public:
		/**
		 * @brief					コンストラクタ
		 * @param game		ゲームクラスの参照
		 */
		TutorialImage(Game& game);
		/**
		 * @brief					デストラクタ
		 */
		~TutorialImage() = default;
		/**
		 * @brief					更新処理
		 */
		void Process() override;
		/**
		 * @brief					描画処理
		 */
		void Draw() override;
		/**
		 * @brief					パラメータの設定
		 * @param tvalue	チュートリアル情報
		 */
		void SetTutorialImage(TutorialValue tvalue);
	private:
		bool _isCol;			//!< 当たり判定によって描画を行うか？
		AABB _collision;	//!< 当たり判定
		/**
		 * @brief					自機と接触しているかの判定
		 */
		void IsCollision();
	};
}

