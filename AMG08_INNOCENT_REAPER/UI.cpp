#include "UI.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ResourceServer.h"
#include "Player.h"
#include "SoulSkin.h"

namespace {
	constexpr auto UI_SPACE_W = 100;

	constexpr auto NUMBER_HP = -1;
	constexpr auto NUMBER_RED = 0;
	constexpr auto NUMBER_BLUE = 1;

	constexpr auto SUB_SOUL = false;
	constexpr auto ADD_SOUL = true;

	constexpr auto SOUL_ANIMA_MAX = 75;
}

namespace inr {

	UI::UI(Game& game) : Image(game) {
		_player = nullptr;
		_pos = { 50, 100 };
		_ghKeys = { ui::KEY_HP };
		_count = { 0 };
		_pal = { 255 };
		_active = { true, true };
		_uiSoul;
		Init();
	}

	void UI::Init() {
		// 各種処理の初期化
	}

	void UI::Process() {
		SoulResearch();
		ActiveCount();

		Dels();
		GraphUpdata();
		// 更新処理
		// 自機が保有する魂の数が変わった場合のみ更新をかける
		// 該当する魂がなくなった場合アニメーションを開始する
	}

	void UI::Draw() {
		DrawEmptyBox();
		for (auto number = 0; number < _count.size(); ++number) {
			auto gh = GraphHandle(_ghKeys[number], _count[number]);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal[number]);
			DrawRotaGraph(_pos.IntX() + ((number + 1) * 100), _pos.IntY(), 1.0, 0, gh, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	void UI::DrawEmptyBox() {
		auto gh = GraphHandle(ui::KEY_BOX, 0);
		DrawRotaGraph(300, 100, 1.0, 0, gh, true);
	}

	void UI::PlayerUpdate() {
		if (_player == nullptr) _player = _game.GetObjectServer()->GetPlayer();
	}

	std::string UI::GetGraphKey(int number) {
		// 返ってきた番号に応じたキーを返す
		switch (number) {
		case NUMBER_RED:
			return ui::KEY_RED;
		case NUMBER_BLUE:
			return ui::KEY_BLUE;
		default:
			return ui::KEY_HP;
		}
	}

	void UI::GraphUpdata() {
		if (_uiSoul.empty()) return;
		auto it = _uiSoul;
		for (auto number = 1; number < static_cast<int>(_ghKeys.size()); ++number) {
			if (_uiSoul.size() < number) break;
			if (_active.at(number) == false) continue;
			_ghKeys.at(number) = GetGraphKey(it.front()->SoulColor());
			it.pop();
		}

		// 輝度がマックスではない場合
		for (auto&& pal : _pal) {
			if (pal == 255) continue;
			if (255 < pal) {
				pal = 255;
				continue;
			}
			pal += 5;
		}
	}

	int UI::GraphHandle(const std::string key, int count) {
		// グラフィックハンドルの算出
		auto interval = 3;
		int no = static_cast<int>(count / interval) % 25;	// 距離を算出;
		return graph::ResourceServer::GetHandles(key, no);
	}

	// アニメーションが終了してから魂を解放するようにする
	// 魂が三つ以上登録されている場合は更新を行わない

	void UI::SoulResearch() {
		auto ps = _player->GetSouls();
		// サイズが違う場合は更新をかける
		if (_uiSoul.size() != ps.size()) {
			// 自機が新しい魂を入手した場合
			if (_uiSoul.size() < ps.size()) {
				auto changes = IsSoulChange(ADD_SOUL);
				auto it = ps;
				auto loopMax = static_cast<int>(ps.size() - changes);
				for (int i = 0; i < loopMax; ++i) it.pop();	// 重複する要素を消す

				
				for (auto i = 0; i < changes; ++i) {
					_ghKeys.emplace_back(GetGraphKey(static_cast<int>(it.back()->SoulColor())));	// ここで止まる
					_count.emplace_back(0);
					_active.emplace_back(true);
					_pal.emplace_back(0);
					it.pop();
				}
				_uiSoul = ps;
				return;
			}
			// 自機の魂が減った場合
			if (ps.size() < _uiSoul.size()) {
				auto changes = IsSoulChange(SUB_SOUL);
				for (auto i = 0; i < changes; ++i) {
					_active[i + 1] = false;	// 非活性状態にする
				}
				_uiSoul = ps;
				return;
			}
		}
		_uiSoul = ps;
	}

	int UI::IsSoulChange(bool value) {
		switch (value) {
		case ADD_SOUL:
			return static_cast<int>(_player->GetSouls().size() - _uiSoul.size());
		case SUB_SOUL:
			return static_cast<int>(_uiSoul.size() - _player->GetSouls().size());
		}
	}

	void UI::Dels() {
		auto usize = static_cast<int>(_count.size());
		for (auto i = 0; i < usize; ++i) {
			if (i == 0)continue;
			if (static_cast<int>(_count.size()) <= i) break;
			int no;
			if (usize != _count.size()) { 
				no = (static_cast<int>(_count.size()) - usize) * -1;
			}
			else no = i;

			if (_count[no] < 75) continue;
			_ghKeys.erase(_ghKeys.begin() + 1);
			_count.erase(_count.begin() + 1);
			_active.erase(_active.begin() + 1);
			_pal.erase(_pal.begin() + 1);
		}
	}

	void UI::ActiveCount() {
		for (auto i = 0; i < _active.size(); ++i) {
			if (_active[i] == true) continue;
			++_count[i];
		}
	}
}
