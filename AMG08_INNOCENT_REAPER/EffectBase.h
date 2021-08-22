#pragma once
#include "Game.h"
#include "Vector2.h"
#include <string>

namespace inr {

	namespace effect {
		constexpr auto NO_COUNT = 0;
		constexpr auto NO_MAXFRAME = 0;

		// キー名
		constexpr auto JUMP = "jump_effect";	// ジャンプエフェクトのキー

		// 画像サイズ
		constexpr auto JUMP_IMAGE = 50;
	}

	class Game;

	// エフェクト
	class EffectBase {
	public:
		EffectBase(Game& game, const Vector2 spawnpos);
		~EffectBase() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();	// 

	private:
		Game& _game;	// ゲーム参照

		Vector2 _position;	// 座標
		std::pair<int, int> _count;	// 左辺:生成時間(フレーム)、生存可能フレーム
		std::string _graphKey;	// グラフィックハンドルのキー

		void GraphResearch(int* gh);	// ResourceServerからグラフィックハンドル取得
		int GetSize(const std::string& key);	// アニメーションの再生フレーム
		void AnimationCount();	// アニメーションカウンタの増減
		int AnimationInterval();	// アニメーションの描画間隔
		int AnimationNumber();	// 現在の描画番号を取得
		bool IsAnimationMax();	// アニメーションは描画し終えたか？
		bool AnimationCountMax();
	};
}

