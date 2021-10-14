/*****************************************************************//**
 * \file   ChipHitCheck.h
 * \brief  チップ番号クラス
 *		   マップチップの登録情報
 * 
 *		   チップヒットチェッククラス
 *		   マップチップの当たり判定を管理する
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Collision.h"

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
		 * @brief	
		 * 
		 * \return 
		 */
		inline int ChipType() { return chipType; }
		// 当たり判定フラグの取得
		inline bool HitType() { return hitType; }
	private:
		int widthMin;		//!< 横幅(Min)
		int widthMax;		//!< 横幅(Max)
		int heightMin;	//!< 縦幅(Min)
		int heightMax;	//!< 縦幅(Max)
		int chipType;		//!< チップの種類
		bool hitType;		//!< 当たり判定はあるか
	};
	// マップチップの当たり判定を管理する
	class ChipHitCheck {
	public:
		// チップ情報を格納する連想配列(キー:チップ番号　値:マップチップの当たり判定)
		using ChipsMap = std::unordered_map<int, ChipNumber>;
		// コンストラクタ
		ChipHitCheck();
		// デストラクタ
		~ChipHitCheck();
		// マップチップの当たり判定登録(引数1:キー　引数2:チップ情報を格納したコンテナ)
		void LoadChipsMap(std::string key, ChipsMap& chipsMap);
		// 読み込むキー情報の更新(引数:切り替え用キー)
		inline void ChangeStageKey(std::string nextkey) { _chipKey = nextkey; }
		// 対象に効果があるかの判定(引数:判定を行うチップ番号)
		int IsChipType(const int no);
		// 対象に当たり判定があるかの判定(引数:判定を行うチップ番号)
		bool IsHitType(const int no);
		// 対象の当たり判定を取得(引数:取得を行うチップ番号)
		AABB ChipCollision(const int no);
	private:
		// (キー:ステージキー　値:対応ステージの当たり判定)
		using StageMaps = std::unordered_map<std::string, ChipsMap>;
		std::string _chipKey;		// 取り出すマップチップ情報(キー)
		StageMaps _stageChipsMap;	// ステージの情報
		// 登録情報の初期化
		void ClearStageMaps();
	};
}

