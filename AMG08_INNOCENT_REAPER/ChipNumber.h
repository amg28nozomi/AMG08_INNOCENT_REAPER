/*****************************************************************//**
 * @file   ChipNumber.h
 * @brief  マップチップの登録情報を保持するチップ番号クラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once

namespace inr {
	/** マップチップのコンパイル時定数 */
	namespace mapchip {
		// マップチップに特殊効果はあるか？
		constexpr auto NONE = -1;				//!< NULL
		constexpr auto NORMAL = 0;			//!< 効果なしの通常チップ
		constexpr auto THORM = 1;				//!< ダメージ判定ありの棘チップ
		constexpr auto IVY = 2;					//!< 掴み判定ありの蔦チップ
		constexpr auto TRANSITION = 3;	//!< ステージ遷移判定ありの遷移チップ
		// 当たり判定はあるか？
		constexpr auto HIT_ON = true;		//! 当たり判定・押し出し処理有り
		constexpr auto HIT_OFF = false;	//! 当たり判定・押し出し処理無し
	}
	/** マップチップの当たり判定 */
	class ChipNumber {
	public:
		/**
		 * @brief	コンストラクタ
		 * @param width1		当たり判定・横幅(min)
		 * @param width2		当たり判定・横幅(max)
		 * @param height1		当たり判定・縦幅(min)
		 * @param height2		当たり判定・縦幅(max)
		 * @param type			マップチップの効果番号
		 * @param ishit			マップチップに当たり・押し出し判定を持たせるか
		 */
		ChipNumber(int width1, int width2, int height1, int height2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		/**
		 * @brief	コンストラクタ
		 * @param width1	当たり判定・横幅(min)
		 * @param width2	当たり判定・横幅(max)
		 * @param type		マップチップの効果番号
		 * @param ishit		マップチップに当たり・押し出し判定を持たせるか
		 */
		ChipNumber(int width1, int width2, int type = mapchip::NORMAL, bool ishit = mapchip::HIT_ON);
		/**
		 * @brief	コンストラクタ
		 * @param type		マップチップの効果番号
		 * @param ishit		マップチップに当たり・押し出し判定を持たせるか
		 */
		ChipNumber(int type, bool ishit = mapchip::HIT_ON);
		/**
		 * @brief	 横幅(min)の取得
		 * @return 横幅(min)を返す
		 */
		inline int WidthMin() { return widthMin; }
		/**
		 * @brief	 横幅(max)の取得
		 * @return 横幅(max)を返す
		 */
		inline int WidthMax() { return widthMax; }
		/**
		 * @brief	 縦幅(min)の取得
		 * @return 縦幅(min)を返す
		 */
		inline int HeightMin() { return heightMin; }
		/**
		 * @brief	 縦幅(max)の取得
		 * @return 縦幅(max)を返す
		 */
		inline int HeightMax() { return heightMax; }
		/**
		 * @brief		効果番号の取得
		 * @return	効果番号の返す
		 */
		inline int ChipType() { return chipType; }
		/**
		 * @brief		判定フラグの取得
		 * @return	判定フラグを返す
		 */
		inline bool HitType() { return hitType; }
	private:
		int widthMin;		//!< 横幅(Min)
		int widthMax;		//!< 横幅(Max)
		int heightMin;	//!< 縦幅(Min)
		int heightMax;	//!< 縦幅(Max)
		int chipType;		//!< チップの効果番号
		bool hitType;		//!< 当たり・押し出し判定の有無
	};
}

