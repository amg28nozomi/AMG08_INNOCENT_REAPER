#pragma once
#include "Image.h"
#include "Vector2.h"
#include <vector>

namespace inr {

	namespace stage {
		namespace number {
			constexpr auto SN_NULL = -1;
			constexpr auto SN_S = 0;
			constexpr auto SN_T = 1;
			constexpr auto SN_1 = 2;
			constexpr auto SN_2 = 3;
			constexpr auto SN_B = 4;
		}
	}

	class BackGround : public Image{
	public:
		BackGround(Game& game);
		
		void Init() override;
		void Process() override;
		void Draw() override;

		void ChangeGraph();	// ステージに応じた画像に切り替える（引数:現在のステージ）
		bool IsChanege();	// キーは切り替わったか？
		int KeyNumber();

		inline void ScrollOff() { _scroll = false; }	// スクロール終了
		inline void ScrollOn() { _scroll = true; }	// スクロール再開
	private:
		std::pair<std::vector<Vector2>, std::vector<Vector2>> _positions;	// 描画座標(左辺:1枚目の描画座標, 右辺2枚目の描画座標)
		std::pair<int, int> _fix;	// 描画修正値
		std::pair<std::vector<double>, std::vector<double>> _scrSpeed;	// スクロール移動量(左辺:x, 右辺:y)
		int _stageNo;	// 現在のステージ
		bool _scroll;	// スクロールするか

		std::string zKey;	// 前景
		void BackDraw();	// 前景の描画
		void BigManage();
		void NormalManage();
		void ScrollY();	// Y座標の修正する
	};
}

