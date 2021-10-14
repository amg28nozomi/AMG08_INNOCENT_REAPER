/*****************************************************************//**
 * \file   GimmickBase.h
 * \brief  ギミックのスーパークラス（オブジェクトベースのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"

namespace inr {
	namespace gimmick {
		// ギミック番号
		constexpr auto NOT_GIMMICK = -1;	// 該当なし
		constexpr auto LEVER = 0;			// レバー
		constexpr auto CRYSTAL = 1;			// 水晶
		constexpr auto BLOCK = 2;			// 壊れる岩
		constexpr auto DOOR = 3;			// ドア
		// ギミックのフラグ
		constexpr auto OFF = false;
		constexpr auto ON = true;
		// レバー
		namespace lever {
			constexpr auto KEY_LEVER = "lever";				// 通常レバー
			constexpr auto KEY_LEVER_BOSS = "boss_lever";	// ボス扉用レバー
			constexpr auto LEVER_SIZE = 140;				// 通常レバーの画像サイズ
			constexpr auto BOSS_LEVER_SIZE = 200;			// ボス扉用レバーの画像サイズ
			// サウンド再生間隔
			constexpr auto SE_LEVER = 50;
		}
		// 岩
		namespace block {
			constexpr auto KEY_BLOCK = "block_key";			// 壊れる岩
			constexpr auto KEY_BREAK = "break_key";			// 壊れるエフェクト
			// 画像サイズ(岩)
			constexpr auto BLOCK_SIZE_W = 140;
			constexpr auto BLOCK_SIZE_H = 280;
			// 画像サイズ(エフェクト)
			constexpr auto BRAKE_SIZE = 500;
			// サウンド
			constexpr auto SE_BREAK = 50;
		}
		// 水晶
		namespace crystal {
			namespace type1 {
				// 画像キー
				constexpr auto KEY_CRYSTAL_EMPTY = "crystal1_empty";	// 通常水晶(空)
				constexpr auto KEY_CRYSTAL_RRD = "crystal1_red";		// 通常水晶(赤)
				constexpr auto KEY_CRYSTAL_BLUE = "crystal1_blue";		// 通常水晶(青)
			}
			namespace type2 {
				// 画像キー
				constexpr auto KEY_CRYSTAL_EMPTY = "crystal2_empty";	// 森用水晶(空)
				constexpr auto KEY_CRYSTAL_RED = "crystal2_red";		// 森用水晶(赤)
				constexpr auto KEY_CRTSTAL_BLUE = "crystal2_blue";		// 森用水晶(青)
			}
			constexpr auto CRYSTAL_SIZE = 140;							// 水晶の画像サイズ
			// サウンド
			constexpr auto KEY_CRYSTAL = "crystal_se";
			constexpr auto SE_CRYSTAL = 40;
			// 魂を取られた
			constexpr auto KEY_ROB = "crystal_rob_se";
			// 魂を与えられた時
			constexpr auto KEY_GIVE = "crystal_give_se";
		}
		// 扉
		namespace door {
			constexpr auto KEY_DOOR_LEVER = "door_lever";				// レバー扉
			constexpr auto KEY_DOOR_BOSS = "door_boss";					// ボス扉
			constexpr auto KEY_DOOR_RED = "door_red";					// 赤扉
			constexpr auto KEY_DOOR_BLUE = "door_blue";					// 青扉
			constexpr auto DOOR_SIZE = 280;								// 画像サイズ
			// サウンド
			constexpr auto KEY_DOOR = "door_se";
			constexpr auto SE_OPEN = 40;
			constexpr auto SE_CLOSE_DOOR = "door_close";	// 扉が閉まる音
		}
	}
	// ギミックのスーパークラス
	class GimmickBase : public ObjectBase {
	protected:
		// ギミックの種類(列挙型)
		enum class GimmickType {
			// 該当なし、レバー、岩、水晶、ドア
			NOT_GIMMICK, LEVER, BLOCK, CRYSTAL, DOOR
		};
		GimmickType _gType;		// ギミックの種類
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		GimmickBase(Game& game);
		// デストラクタ
		~GimmickBase();
		// 初期化
		virtual void Init() override;
		// 更新
		virtual void Process() override;
		// 描画
		virtual void Draw() override;
		// オブジェクト情報の更新
		virtual void ObjValueUpdate();
		// ギミック番号の取得
		int GimmickType();
	};
}

