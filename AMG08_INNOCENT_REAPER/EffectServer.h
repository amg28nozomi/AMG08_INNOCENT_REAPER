/*****************************************************************//**
 * @file   EffectServer.h
 * @brief  エフェクトサーバクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include <memory>

namespace inr {
	/** エフェクト */
	namespace effect {
		/** 種類 */
		namespace type {
			// エフェクトの描画順番
			constexpr auto FORMER = 0;	//!< オブジェクトより前面に描画
			constexpr auto BACK = 1;		//!< オブジェクトより後面に描画
		}
	}
	/** 二重インクルード防止 */
	class Game;	
	class EffectBase;
	/** エフェクトの管理を行うサーバクラス */
	class EffectServer {
	public:
		/**
		 * @brief				コンストラクタ
		 * @param game	ゲームクラスの参照
		 */
		EffectServer(Game& game);
		// デストラクタ
		~EffectServer();
		// 初期化
		void Init();
		// 更新
		void Process();	
		// 前景描画
		void DrawFormer();
		// 後景描画
		void DrawBack();
		// エフェクトの登録(引数1:登録エフェクト　引数2:どちらに登録するか)
		void Add(std::unique_ptr<EffectBase> eff, int type);
		// 消去フラグをオンにする
		inline void DelOn() { if (_del != true) _del = true; }
	private:
		Game& _game;											// ゲームクラス参照
		bool _isAdd;											// 新規エフェクトが追加されたか？
		bool _update;											// 更新を行っているか？
		bool _del;												// 登録エフェクトの消去処理を呼び出すかの判定
		std::vector<std::unique_ptr<EffectBase>> _effectsF;		// 前景エフェクト格納用コンテナ
		std::vector<std::unique_ptr<EffectBase>> _effectsB;		// 後景エフェクト格納用コンテナ(後描画)
		std::vector<std::unique_ptr<EffectBase>> _addFormer;	// 一時的格納用コンテナ(前景用)
		std::vector<std::unique_ptr<EffectBase>> _addBack;		// 一時的格納用コンテナ(後景用)
		// 各種コンテナの解放処理
		void Clears();
		// 登録エフェクトの消去処理
		void IsDelete();
		// 一時的格納用コンテナに登録されているエフェクトの所有権を該当するコンテナに移行する
		void MoveAddEffects();
	};
}

