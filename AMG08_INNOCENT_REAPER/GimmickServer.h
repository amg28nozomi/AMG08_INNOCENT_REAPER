/*****************************************************************//**
 * \file   GimmickServer.h
 * \brief  ギミックサーバクラス
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
	// 二重インクルード対策
	class Game;
	class GimmickBase;
	// ギミックサーバ
	// ギミックベースオブジェクトの管理を行う
	class GimmickServer {
	public:
		GimmickServer(Game& game);
		~GimmickServer();
		// 更新
		void Process();
		// 描画
		void Draw();	
		// ギミックの登録(引数1:ギミック)
		void Add(std::shared_ptr<GimmickBase> gimmick);
		// ギミックの削除
		void Del();
		// ギミックが登録されているコンテナを取得
		inline std::vector<std::shared_ptr<GimmickBase>> GetGimmicks() { return _gimmicks; }
		// コンテナの解放
		inline void Clear() { _gimmicks.clear(); }
		// 消去フラグを真にする
		inline void DelOn() { _isDel = true; }
	private:
		Game& _game;											// ゲーム参照
		bool _isDel;											// オブジェクトを消去するか否か
		std::vector<std::shared_ptr<GimmickBase>> _gimmicks;	// ギミックを管理するコンテナ
	};
}

