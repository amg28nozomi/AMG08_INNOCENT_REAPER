#pragma once
#include <vector>
#include <memory>

namespace inr {

	namespace effect {
		namespace type {
			constexpr auto FORMER = 0;
			constexpr auto BACK = 1;
		}
	}

	class Game;
	class EffectBase;

	class EffectServer {
	public:
		EffectServer(Game& game);
		~EffectServer();	// 配列初期化

		void Init();	// 初期化
		void Process();	// 更新
		void DrawFormer();	// 描画
		void DrawBack();

		void Add(std::unique_ptr<EffectBase> eff, int type);	// 引数1:エフェクト　引数2:どちらに登録するか
		inline void DelOn() { if (_del != true) _del = true; }	// 消去フラグをオンにする
	private:
		Game& _game;
		bool _isAdd;	// 新規エフェクトが追加されたか？
		bool _update;	// 更新を行っているか？
		bool _del;	// 消去するか否か
		std::vector<std::unique_ptr<EffectBase>> _effectsF;	// エフェクト補間用
		std::vector<std::unique_ptr<EffectBase>> _effectsB;

		std::vector<std::unique_ptr<EffectBase>> _addFormer;
		std::vector<std::unique_ptr<EffectBase>> _addBack;

		void Clears();	// 初期化
		void IsDelete();
		bool IsEmpty();
	};
}

