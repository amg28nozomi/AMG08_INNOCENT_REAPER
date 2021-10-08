#pragma once
#include <vector>
#include <memory>

namespace inr {
	namespace effect {
		namespace type {
			// エフェクトの描画順番
			constexpr auto FORMER = 0;	// オブジェクトより前面に描画
			constexpr auto BACK = 1;	// オブジェクトより後面に描画
		}
	}
	// 二重インクルード防止
	class Game;	
	class EffectBase;
	// エフェクトの管理を行うサーバクラス
	class EffectServer {
	public:
		EffectServer(Game& game);
		~EffectServer();	// 配列初期化
		// 初期化
		void Init();
		// 更新
		void Process();	
		// 前描画
		void DrawFormer();
		// 後描画
		void DrawBack();
		// エフェクトの登録(引数1:登録エフェクト　引数2:どちらに登録するか)
		void Add(std::unique_ptr<EffectBase> eff, int type);
		// 消去フラグをオンにする
		inline void DelOn() { if (_del != true) _del = true; }
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

