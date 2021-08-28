#pragma once
#include "Image.h"
#include "Vector2.h"
#include <vector>

namespace inr {

	class BackGround : public Image{
	public:
		BackGround(Game& game);
		
		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		std::pair<std::vector<Vector2>, std::vector<Vector2>> _positions;	// 描画座標(左辺:1枚目の描画座標, 右辺2枚目の描画座標)
		std::vector<double> _scrSpeed;	// スクロール移動量
	};
}

