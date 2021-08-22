#pragma once
#include "ObjectBase.h"

namespace inr {

	class SoulCursor : public ObjectBase {
	public:
		SoulCursor(Game& game);
		~SoulCursor() = default;

		void Init() override;
		void Process() override;		// 更新
		void Draw() override;	// 描画
	private:
		Vector2 _moveVector;
		void Move(int lever);	// 入力／移動処理
		void PositionUpdate();
	};
}

