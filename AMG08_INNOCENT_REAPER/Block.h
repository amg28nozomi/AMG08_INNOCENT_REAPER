#pragma once
#include "GimmickBase.h"

namespace inr {
	// 壊れたかどうかのフラグ
	namespace gimmick {
		namespace block {
			constexpr auto BRAKE_ON = true;	// 破壊済み
			constexpr auto BRAKE_OFF = false;	// 未破壊
		}
	}
	// 壊れる岩
	class Block : public GimmickBase {
	public:
		Block(Game& game);
		~Block() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(ObjectValue objValue) override;	// オブジェクト情報の登録
		void ObjValueUpdate() override;	// オブジェクト情報の更新

		inline bool IsBreak() { return _break; }	// オブジェクトは壊れているか？
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection);	// 押し出し処理
		bool Break();	// 破壊処理
	private:
		int _pal;	// 透明度
		bool _break;	// 壊されたか否か
	};
}

