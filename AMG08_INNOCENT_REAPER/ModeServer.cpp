/*****************************************************************//**
 * @file   ModeServer.cpp
 * @brief  各種モードを管理するモードサーバ
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ModeServer.h"
#include "ModeBase.h"
#include "ModeTitle.h"
#include "ModeMain.h"
#include "ModeEnd.h"
#include "Game.h"
#include "ObjectServer.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace {
	constexpr auto MODE_NULL = "null";
	constexpr auto CHANGE_INTERVAL = 60;
}

namespace inr {

	ModeServer::ModeServer(Game& game) : _game(game) {
		Init();
	}

	ModeServer::~ModeServer() {
		ClearModeLists();
	}

	void ModeServer::Init() {
		// 連想配列初期化
		ClearModeLists();
		// 各種モード登録
		_modes.emplace(mode::TITLE, std::make_shared<ModeTitle>(_game.GetGame()));	// タイトル
		// セレクト（未実装）
		_modes.emplace(mode::MAIN, std::make_shared<ModeMain>(_game.GetGame()));	// ゲーム本編
		_modes.emplace(mode::FIN, std::make_shared<ModeEnd>(_game.GetGame()));	// プログラム終了前の処理
		_fadeBlack = std::make_unique<FadeBlack>(_game.GetGame());
		// 検索キーをTitleModeに設定
#ifdef _DEBUG
		// _modeKey = mode::MAIN;
		_modeKey = mode::TITLE;
#endif
#ifndef _DEBUG
		_modeKey = mode::TITLE;
#endif
		_ChangeKey = MODE_NULL;
		ModeInit();
	}

	void ModeServer::Process() {
		// Modeの更新用キーが変更されているかどうか
		IsModeChange();
		// 現在設定されているモードを取り出し、更新処理を実行。
		auto mode = _modes.find(_modeKey);
		if (mode == _modes.end()) return;
		mode->second->Process();
		_fadeBlack->Process();
	}

	void ModeServer::Draw() {
		// 現在のモードを取り出し、描画処理を実行。
		auto mode = _modes.find(_modeKey);
		if (mode == _modes.end()) return;
		mode->second->Draw();
		_fadeBlack->Draw();
	}

	void ModeServer::ClearModeLists() {
		_modes.clear();
	}

	void ModeServer::ModeChange(std::string nextMode, int interval) {
		_ChangeKey = nextMode;	// モード切り替えフラグオン

		if (interval == 0) interval = CHANGE_INTERVAL;
		_fadeBlack->FlagChange(image::FADE_OUT, interval);
	}

	bool ModeServer::ModeInit() {
		auto mode = _modes.find(_modeKey);				// 現在のモードを取り出す
		if (mode == _modes.end()) return false;		// ヒットしなかった（初期化失敗）
		mode->second->Init();											// 初期化処理実行
		return true;
	}

	void ModeServer::IsModeChange() {
		if (_ChangeKey == MODE_NULL) return;		// フラグがオフの場合は抜ける
		// 処理が切り替わった瞬間に一度だけオブジェクトの再配置等を行う
		if (_fadeBlack->PalChange() == true) {
			ModeInit();														// 現在のモードを初期化
			_game.GetObjectServer()->AllClear();
			_modeKey = _ChangeKey;								// 更新が入っている場合は実行用キーを更新
			ModeInit();														// 次のモードを初期化
			_ChangeKey = MODE_NULL;								// 更新用キーを初期化
		}
	}

	void ModeServer::GameEnd() {
		// ゲームを終了する
		_fadeBlack->FlagChange(image::FADE_OUT, CHANGE_INTERVAL);
		_game.ProgramEnd();
	}

	std::shared_ptr<ModeMain> ModeServer::GetModeMain() {
		auto mode = _modes.find(mode::MAIN);
		return std::dynamic_pointer_cast<ModeMain>(mode->second);
	}
}