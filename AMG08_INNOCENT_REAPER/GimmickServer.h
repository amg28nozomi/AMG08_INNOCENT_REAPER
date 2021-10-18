/*****************************************************************//**
 * @file   GimmickServer.h
 * @brief  ギミックサーバクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
	/**  二重インクルード防止 */
	class Game;
	class GimmickBase;
	/** ギミックの管理を行う */
	class GimmickServer {
	public:
		/**
		 * @brief					コンストラクタ
		 * @param game		ゲームクラスの参照
		 */
		GimmickServer(Game& game);
		/**
		 * @brief					デストラクタ
		 */
		~GimmickServer();
		/**
		 * @brief					更新処理
		 */
		void Process();
		/**
		 * @brief					描画処理
		 */
		void Draw();	
		/**
		 * @brief					ギミックの登録
		 * @param gimmick	登録ギミックのシェアードポインタ
		 */
		void Add(std::shared_ptr<GimmickBase> gimmick);
		/**
		 * @brief					ギミックの削除
		 */
		void Del();
		/**
		 * @brief					コンテナの開放
		 */
		inline void Clear() { _gimmicks.clear(); }
		/**
		 * @brief					消去フラグをオンにする
		 */
		inline void DelOn() { _isDel = true; }
		/**
		 * @brief					全ギミックの取得
		 * @return				ギミックが登録されているコンテナを返す
		 */
		inline std::vector<std::shared_ptr<GimmickBase>> GetGimmicks() { return _gimmicks; }
	private:
		Game& _game;																					//!< ゲームクラスの参照
		bool _isDel;																					//!< オブジェクトを消去するか否か
		std::vector<std::shared_ptr<GimmickBase>> _gimmicks;	//!< ギミックを管理するコンテナ
	};
}

