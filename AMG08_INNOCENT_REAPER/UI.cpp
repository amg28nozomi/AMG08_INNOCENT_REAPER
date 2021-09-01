#include "UI.h"
#include "Game.h"
#include "ObjectServer.h"
#include "Player.h"
#include "SoulSkin.h"
#include <queue>

namespace {
	constexpr auto UI_SPACE_W = 100;
}

namespace inr {

	UI::UI(Game& game) : Image(game) {
		_player = _game.GetObjectServer()->GetPlayer();
		_pos = { 300, 200 };
		_ghKeys = { ui::KEY_HP };
		_count = { 0 };
		Init();
	}

	void UI::Init() {
		// 各種処理の初期化
	}

	void UI::Process() {

		auto ps = _player->GetSouls();
		if (ps.empty()) {
			// 魂が空の場合は何も行わないよ
		}


		// 更新処理
		// 自機が保有する魂の数が変わった場合のみ更新をかける
		// 該当する魂がなくなった場合アニメーションを開始する
	}

	void UI::Draw() {
		for (auto number = 0; number < _player->GetSouls().size() + 1; ++number) {
			DrawRotaGraph(_pos.IntX() * number, _pos.IntY(), 1.0, 0, )
			// 初回のみ脱出
			if (number == 0) continue;
		}
	}
}
