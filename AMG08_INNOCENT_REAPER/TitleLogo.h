#pragma once
#include "Image.h"
#include "Collision.h"
#include "RedGreenBlue.h"
#include <vector>

namespace inr {
	class TitleLogo : public Image {
	public:
		TitleLogo(Game& game);
		
		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		Vector2 _position2;
		Collision _hitCol1;	// 当たり判定
		Collision _hitCol2;

		int _pal;	// 
		std::vector<double> _rate;	// 拡大率
		std::vector<int> _rateFlag;
		std::string _graphKey2;	// もう一つのグラハン

		bool RateUpdate();	// 拡大率の更新

#ifdef _DEBUG
		void DebugBox(Collision col);
#endif
		void CollisionOut();
	};
}

