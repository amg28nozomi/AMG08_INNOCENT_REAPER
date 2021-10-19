/*****************************************************************//**
 * @file   TitleLogo.h
 * @brief  タイトルロゴ（画像クラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "Collision.h"
#include "RedGreenBlue.h"
#include <vector>

namespace inr {
	/** タイトルロゴ */
	class TitleLogo : public Image {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		TitleLogo(Game& game);
		/**
		 * @brief				初期化処理
		 */
		void Init() override;
		/**
		 * @brief				更新処理
		 */
		void Process() override;
		/**
		 * @brief				描画処理
		 */
		void Draw() override;
	private:
		Vector2 _position2;						//!< 座標
		Collision _hitCol1;						//!< 当たり判定ボックス(1)
		Collision _hitCol2;						//!< 当たり判定ボックス(2)
		int _pal;											//!< 輝度
		std::vector<double> _rate;		//!< 拡大率
		std::vector<int> _rateFlag;		//!< 拡大フラグ
		std::string _graphKey2;				//!< 画像キー
		/**
		 * @brief				拡大率の更新
		 * @return			trueを返す
		 */
		bool RateUpdate();
#ifdef _DEBUG
		/**
		 * @brief				当たり判定ボックスの描画
		 * @param col		描画対象ボックス
		 */
		void DebugBox(Collision col);
#endif
		/**
		 * @brief				衝突判定フラグの停止
		 */
		void CollisionOut();
	};
}

