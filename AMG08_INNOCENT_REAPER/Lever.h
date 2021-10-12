/*****************************************************************//**
 * \file   Lever.h
 * \brief  レバークラス（ギミックベースクラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	namespace gimmick {
		namespace door {
			// ドア番号
			constexpr auto NOT_DOOR = -1;	// 該当なし
			constexpr auto D_LEVER = 1;		// レバー
			constexpr auto D_RED = 2;		// 赤
			constexpr auto D_BLUE = 3;		// 青
			constexpr auto D_BOSS = 4;		// ボス
		}
	}
	// 二重インクルード防止
	class Door;
	// レバークラス
	class Lever : public GimmickBase {
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		Lever(Game& game);
		// デストラクタ
		~Lever() = default;
		// 更新
		void Process() override;
		// 描画
		void Draw() override;
		// ドアの解放処理
		void OpenDoor();
		// オブジェクト情報の登録
		void SetParameter(ObjectValue objValue) override;
		// オブジェクト情報の更新
		void ObjValueUpdate() override;
	private:
		std::shared_ptr<Door> _door;	// 扉
	};

}