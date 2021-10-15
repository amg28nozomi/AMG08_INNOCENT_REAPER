/*****************************************************************//**
 * @file   GimmickBase.h
 * @brief  ギミックのスーパークラス（オブジェクトベースのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"

namespace inr {
	/** ギミックのコンパイル時定数 */
	namespace gimmick {
		// ギミック番号
		constexpr auto NOT_GIMMICK = -1;	//!< 該当なし
		constexpr auto LEVER = 0;					//!< レバー
		constexpr auto CRYSTAL = 1;				//!< 水晶
		constexpr auto BLOCK = 2;					//!< 壊れる岩
		constexpr auto DOOR = 3;					//!< ドア
		// ギミックのフラグ
		constexpr auto OFF = false;				//!< ギミックオフ
		constexpr auto ON = true;					//!< ギミックオン
		/** レバーのコンパイル時定数 */
		namespace lever {
			constexpr auto KEY_LEVER = "lever";						//!< 通常レバー
			constexpr auto KEY_LEVER_BOSS = "boss_lever";	//!< ボス扉用レバー
			constexpr auto LEVER_SIZE = 140;							//!< 通常レバーの画像サイズ
			constexpr auto BOSS_LEVER_SIZE = 200;					//!< ボス扉用レバーの画像サイズ
			// サウンド再生間隔
			constexpr auto SE_LEVER = 50;									//!< SEの再生間隔
		}
		/** 壊れる岩のコンパイル時定数 */
		namespace block {
			constexpr auto KEY_BLOCK = "block_key";				//!< 壊れる岩
			constexpr auto KEY_BREAK = "break_key";				//!< 破壊エフェクト
			// 画像サイズ(岩)
			constexpr auto BLOCK_SIZE_W = 140;						//!< 壊れる岩の幅
			constexpr auto BLOCK_SIZE_H = 280;						//!< 壊れる岩の高さ
			// 画像サイズ(エフェクト)
			constexpr auto BRAKE_SIZE = 500;							//!< 破壊エフェクトのサイズ
			// サウンド
			constexpr auto SE_BREAK = 50;									//!< SEの再生間隔
		}
		/** 水晶のコンパイル時定数 */
		namespace crystal {
			/** 通常 */
			namespace type1 {
				// 画像キー
				constexpr auto KEY_CRYSTAL_EMPTY = "crystal1_empty";	//!< 通常水晶(空)
				constexpr auto KEY_CRYSTAL_RRD = "crystal1_red";			//!< 通常水晶(赤)
				constexpr auto KEY_CRYSTAL_BLUE = "crystal1_blue";		//!< 通常水晶(青)
			}
			/** 森用 */
			namespace type2 {
				// 画像キー
				constexpr auto KEY_CRYSTAL_EMPTY = "crystal2_empty";	//!< 森用水晶(空)
				constexpr auto KEY_CRYSTAL_RED = "crystal2_red";			//!< 森用水晶(赤)
				constexpr auto KEY_CRTSTAL_BLUE = "crystal2_blue";		//!< 森用水晶(青)
			}
			constexpr auto CRYSTAL_SIZE = 140;											//!< 水晶のサイズ
			// サウンド
			constexpr auto KEY_CRYSTAL = "crystal_se";							//!< 水晶SE
			constexpr auto SE_CRYSTAL = 40;													//!< SEの再生間隔
			// SE
			constexpr auto KEY_ROB = "crystal_rob_se";							//!< 魂を奪われた
			constexpr auto KEY_GIVE = "crystal_give_se";						//!< 魂を与えられた
		}
		/** ドアのコンパイル時定数 */
		namespace door {
			constexpr auto KEY_DOOR_LEVER = "door_lever";						//!< レバー扉
			constexpr auto KEY_DOOR_BOSS = "door_boss";							//!< ボス扉
			constexpr auto KEY_DOOR_RED = "door_red";								//!< 赤扉
			constexpr auto KEY_DOOR_BLUE = "door_blue";							//!< 青扉
			constexpr auto DOOR_SIZE = 280;													//!< ドアのサイズ
			// サウンド
			constexpr auto KEY_DOOR = "door_se";										//!< 開ける
			constexpr auto SE_OPEN = 40;														//!< SEの再生間隔
			constexpr auto SE_CLOSE_DOOR = "door_close";						//!< 閉じる
		}
	}
	/** ギミックのスーパークラス */
	class GimmickBase : public ObjectBase {
	protected:
		/**
		 * @brief ギミックの種類を表す列挙型
		 *				該当なし、レバー、岩、水晶、ドア
		 */
		enum class GimmickType {
			NOT_GIMMICK, LEVER, BLOCK, CRYSTAL, DOOR
		};
		GimmickType _gType;		//!< ギミックの種類
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		GimmickBase(Game& game);
		/**
		 * @brief				デストラクタ
		 */
		~GimmickBase();
		/**
		 * @brief				初期化処理
		 * 
		 */
		virtual void Init() override;
		/**
		 * @brief				更新処理
		 * 
		 */
		virtual void Process() override;
		/**
		 * @brief				描画処理
		 * @brief				描画処理
		 */
		virtual void Draw() override;
		/**
		 * @brief				オブジェクト情報の更新
		 */
		virtual void ObjValueUpdate();
		/**
		 * @brief				ギミック番号の取得
		 * @return			ギミック番号を返す
		 */
		int GimmickType();
	};
}

