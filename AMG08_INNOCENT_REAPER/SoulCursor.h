#pragma once
#include "ObjectBase.h"
#include "RedGreenBlue.h"

namespace inr {

	namespace ui {
		constexpr auto CURSOR = "ui_cursor";
	}

	class SoulCursor : public ObjectBase {
	public:
		SoulCursor(Game& game);
		~SoulCursor() = default;

		void Init() override;
		void Process() override;		// 更新
		void Draw() override;	// 描画
	private:
		Vector2 _moveVector;
		RedGreenBlue _rgb;
		bool _input;	// 入力は可能か？

		bool _setBlend;	// 色彩は調整したか？

		void Move(int lever1, int lever2);	// 入力／移動処理
		void PositionUpdate();
		void ChangeBlendGraph();	// 種類に応じた輝度の変更
	};
}

