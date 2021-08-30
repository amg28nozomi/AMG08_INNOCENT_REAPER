#include "ModeServer.h"
#include "ModeBase.h"
#include "ModeTitle.h"
#include "ModeMain.h"
#include "ModeEnd.h"
#include "Game.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace {
	constexpr auto MODE_NULL = "null";
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
		_modes.emplace(mode::TITLE, std::make_unique<ModeTitle>(_game.GetGame()));	// タイトル
		// セレクト（未実装）
		_modes.emplace(mode::MAIN, std::make_unique<ModeMain>(_game.GetGame()));	// ゲーム本編
		_modes.emplace(mode::FIN, std::make_unique<ModeEnd>(_game.GetGame()));	// プログラム終了前の処理
		
		// 検索キーをTitleModeに設定
		_modeKey = mode::MAIN;

		//_modeKey = mode::TITLE;
		_ChangeKey = MODE_NULL;
		ModeInit();
	}

	void ModeServer::Process() {
		// Modeの更新用キーが変更されているかどうか
		if (_ChangeKey != MODE_NULL) {
			// 現在のモードを初期化
			ModeInit();
			// 更新が入っている場合は実行用キーを更新
			_modeKey = _ChangeKey;
			// 次のモードを初期化
			ModeInit();
			// 更新用キーを初期化
			_ChangeKey = MODE_NULL;
		}

		// 現在設定されているモードを取り出し、更新処理を実行。
		auto mode = _modes.find(_modeKey);
		if (mode == _modes.end()) return;
		mode->second->Process();
	}

	void ModeServer::Draw() {
		// 現在のモードを取り出し、描画処理を実行。
		auto mode = _modes.find(_modeKey);
		if (mode == _modes.end()) return;
		mode->second->Draw();
	}

	void ModeServer::ClearModeLists() {
		_modes.clear();
	}

	bool ModeServer::ModeInit() {
		// 現在のモードを取り出す
		auto mode = _modes.find(_modeKey);
		// ヒットしなかった（初期化失敗）
		if (mode == _modes.end()) return false;
		// 初期化処理実行
		mode->second->Init();
		return true;
	}

	std::unique_ptr<ModeBase>& ModeServer::GetMode(std::string modeKey) {
		auto mode = _modes.find(modeKey);
		return mode->second;
	}
}