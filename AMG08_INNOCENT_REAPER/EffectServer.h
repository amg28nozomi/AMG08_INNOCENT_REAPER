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
		/**
		 * @brief				デストラクタ
		 */
		~EffectServer();
		/**
		 * @brief				初期化処理
		 */
		void Init();
		/**
		 * @brief				更新処理
		 */
		void Process();	
		/**
		 * @brief				前景描画
		 */
		void DrawFormer();
		/**
		 * @brief				後景描画
		 */
		void DrawBack();
		/**
		 * @brief				エフェクトの登録
		 * @param eff		登録するエフェクトのユニークポインタ
		 * @param type	登録タイプ(0:前景エフェクト　1:後景エフェクト)
		 */
		void Add(std::unique_ptr<EffectBase> eff, int type);
		/**
		 * @brief				消去フラグをオンにするか
		 */
		inline void DelOn() { if (_del != true) _del = true; }
	private:
		Game& _game;																					//!< ゲームクラス参照
		bool _isAdd;																					//!< 新規エフェクトが追加されたか？
		bool _update;																					//!< 更新を行っているか？
		bool _del;																						//!< 登録エフェクトの消去処理を呼び出すかの判定
		std::vector<std::unique_ptr<EffectBase>> _effectsF;		//!< 前景エフェクト格納用コンテナ
		std::vector<std::unique_ptr<EffectBase>> _effectsB;		//!< 後景エフェクト格納用コンテナ(後描画)
		std::vector<std::unique_ptr<EffectBase>> _addFormer;	//!< 一時的格納用コンテナ(前景用)
		std::vector<std::unique_ptr<EffectBase>> _addBack;		//!< 一時的格納用コンテナ(後景用)
		/**
		 * @brief	各種コンテナの解放処理
		 */
		void Clears();
		/**
		 * @brief	登録エフェクトの消去処理
		 */
		void IsDelete();
		/**
		 * @brief 一時的格納用コンテナに登録されているエフェクトの所有権を該当するコンテナに移行する
		 */
		void MoveAddEffects();
	};
}

