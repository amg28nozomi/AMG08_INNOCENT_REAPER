/*****************************************************************//**
 * \file   Block.h
 * \brief  壊れる岩クラス（ギミックベースクラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {
	namespace gimmick {
		namespace block {
			// 壊れたかどうかのフラグ
			constexpr auto BRAKE_ON = true;		// 破壊済み
			constexpr auto BRAKE_OFF = false;	// 未破壊
		}
	}
	// 壊れる岩(ギミック)
	class Block : public GimmickBase {
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		Block(Game& game);
		// デストラクタ
		~Block() = default;
		// 更新
		void Process() override;
		// 描画
		void Draw() override;
		// オブジェクト情報の登録
		void SetParameter(ObjectValue objValue) override;
		// オブジェクト情報の更新
		void ObjValueUpdate() override;
		// オブジェクトは壊れているか？
		inline bool IsBreak() { return _break; }
		// 押し出し処理
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection);
		// 破壊処理
		bool Break();
	private:
		int _pal;		// 透明度
		bool _break;	// 壊されたか否か
	};
}

