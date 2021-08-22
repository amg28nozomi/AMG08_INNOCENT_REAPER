#pragma once
#include "Game.h"
#include "Vector2.h"

namespace inr {

	class Game;

	// エフェクト
	class EffectBase {
	public:
		EffectBase(Game& game, const Vector2 spawnpos);
		~EffectBase() = default;

		virtual void Init();
		virtual void Process();
		void Draw();
	private:
		Game& _game;	// ゲーム参照

		Vector2 _position;	// 座標
		std::pair<int, int> _count;	// 左辺:生成時間(フレーム)、生存可能フレーム
	};
}

