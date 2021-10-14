/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  モードのスーパークラス
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>

namespace inr {
	namespace mode {
		// 各種モードのキー情報
		constexpr auto TITLE = "mode_title";	// タイトル
		constexpr auto MAIN = "mode_main";		// 本編
		constexpr auto FIN = "mode_end";		// 終了
	}
	// 二重インクルード防止
	class Game;
	class ObjectServer;
	class ModeServer;
	// モードベース
	class ModeBase {
	protected:
		static int _modeFrame;	// 経過フレーム
		bool _update;			// 処理を更新するかどうか
		bool _resetFlg;			// モード切り替え時に初期化するかどうか
		Game& _game;			// ゲームクラス参照
		std::string _bgmKey;	// bgmのキー
		// BGMの管理
		bool BgmManage();
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		ModeBase(Game& game);
		// デストラクタ
		~ModeBase();
		// 初期化
		virtual void Init();
		// 更新
		virtual void Process();	
		// 描画
		virtual void Draw();
		// ステージキーの切り替え(引数:次のステージキー)
		virtual void ChangeKey(const std::string nextStage);
		// 初期化フラグのオン
		inline void Reset() { _resetFlg = true; }
		// 経過フレームの初期化
		inline void TimeClear() { _modeFrame = 0; };
		// 経過フレームを取得
		inline int GetFrame() { return _modeFrame; }
		// モードの取得
		virtual inline std::string GetKey() { return ""; }
	};
}