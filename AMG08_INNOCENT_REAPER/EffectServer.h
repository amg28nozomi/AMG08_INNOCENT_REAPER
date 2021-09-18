#pragma once
#include <vector>
#include <memory>

namespace inr {

	class Game;
	class EffectBase;

	class EffectServer {
	public:
		EffectServer(Game& game);
		~EffectServer();	// 配列初期化

		void Init();	// 初期化
		void Process();	// 更新
		void Draw();	// 描画

		void Add(std::unique_ptr<EffectBase> eff);
		inline void DelOn() { if (_del != true) _del = true; }	// 消去フラグをオンにする
	private:
		Game& _game;
		bool _update;	// 更新を行っているか？
		bool _del;	// 消去するか否か
		std::vector<std::unique_ptr<EffectBase>> _effects;	// エフェクト補間用
		std::vector<std::unique_ptr<EffectBase>> _addEffects;	// 一時追加用

		void Clears();	// 初期化
		void IsDelete();
	};
}

