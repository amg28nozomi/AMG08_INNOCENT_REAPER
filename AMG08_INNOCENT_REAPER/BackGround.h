#pragma once
#include "Image.h"
#include "Vector2.h"
#include <vector>

namespace inr {

	namespace stage {
		// ステージ番号
		namespace number {
			constexpr auto SN_NULL = -1;	// 該当なし
			constexpr auto SN_S = 0;		// ステージ0
			constexpr auto SN_T = 1;		// チュートリアルステージ
			constexpr auto SN_1 = 2;		// ステージ1
			constexpr auto SN_2 = 3;		// ステージ2
			constexpr auto SN_B = 4;		// ボスステージ
		}
	}

	// 背景
	class BackGround : public Image{
	public:
		BackGround(Game& game);
		
		// 初期化
		void Init() override;
		// 更新
		void Process() override;
		//描画
		void Draw() override;

		// ステージに応じた画像に切り替える（引数:現在のステージ）
		void ChangeGraph();
		// キーは切り替わったか？
		bool IsChanege();
		// ステージ番号の判定
		int KeyNumber();

		// スクロール終了
		inline void ScrollOff() { _scroll = false; }
		// スクロール再開
		inline void ScrollOn() { _scroll = true; }
	private:
		std::pair<std::vector<Vector2>, std::vector<Vector2>> _positions;	// 描画座標(左辺:1枚目の描画座標, 右辺2枚目の描画座標)
		std::pair<int, int> _fix;	// 描画修正値
		std::pair<std::vector<double>, std::vector<double>> _scrSpeed;	// スクロール移動量(左辺:x, 右辺:y)
		int _stageNo;		// 現在のステージ
		bool _scroll;		// スクロールするか

		std::string zKey;	// 前景
		// 前描画
		void BackDraw();
		// スクロール処理(大)
		void BigManage();
		// スクロール処理(小)
		void NormalManage();
		// Y座標の修正
		void ScrollY();
	};
}

