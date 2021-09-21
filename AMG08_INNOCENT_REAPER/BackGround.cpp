#include "BackGround.h"
#include "Vector2.h"
#include "ResourceServer.h"
#include "Game.h"
#include "MapChips.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include <DxLib.h>

namespace {
	constexpr auto BACKGROUND_SPEED = 0.5;
	constexpr auto MAX_SCROLL = 1920 + 960;
	constexpr auto MIN_SCROLL = -960;


	constexpr auto STAGE1_BACK_00 = 0.30;
	constexpr auto STAGE1_BACK_01 = 0.4;
	constexpr auto STAGE1_BACK_02 = 0.6;

	constexpr auto STAGE2_BACK_00 = 0.25;
	constexpr auto STAGE2_BACK_01 = 0.40;
	constexpr auto STAGE2_BACK_02 = 0.50;
	constexpr auto STAGE2_BACK_03 = 0.60;
}

namespace inr {

	BackGround::BackGround(Game& game) : Image(game){
		_stageNo = -1;
		_pos = { 960 , 1080 };
		_fix = { 0, 0 };
		_scroll = true;
		// ChangeGraph();	// 最初のキーを読み込み
	}

	void BackGround::Init() {
		// スクリーン座標のサイズ分だけ初期化を行う
		for (int n = 0; n < static_cast<int>(_scrSpeed.first.size()); ++n) {
			_positions.first.emplace_back(HALF_WINDOW_W, _fix.first);
			_positions.second.emplace_back(MAX_SCROLL, _fix.second);
		}
		ScrollY();	// 修正をかける
	}

	void BackGround::Process() {
		ChangeGraph();
		if (_graphKey == background::BACK_GROUND_B) NormalManage();
		else BigManage();
	}

	void BackGround::Draw() {
		for (auto number = 0; number < static_cast<int>(_positions.first.size()); ++number) {
			auto x1 = _positions.first[number].IntX();
			auto y1 = _positions.first[number].IntY();

			auto x2 = _positions.second[number].IntX();
			auto y2 = _positions.second[number].IntY();
			auto gh = graph::ResourceServer::GetHandles(_graphKey, number);
			DrawRotaGraph(x1, y1, 1.0, 0, gh, true, false);
			DrawRotaGraph(x2, y2, 1.0, 0, gh, true, false);
			// DrawFormatString(500, number * 100, GetColor(255, 0, 255), "backGround_y : %d\n", _positions.second[number].IntY());
		}
		if (_game.GetModeServer()->GetModeMain()->StageKey() == stage::STAGE_0) BackDraw();
	}

	void BackGround::ChangeGraph() {
		if (IsChanege() != true) return;
		_graphKey.clear();
		_scrSpeed.first.clear();	// 中身を空にする
		_scrSpeed.second.clear();
		_positions.first.clear();
		_positions.second.clear();
		// 現在のステージに応じて各種値を更新
		switch (_stageNo) {
		case stage::number::SN_S:	// ステージS
			_graphKey = background::BACK_GROUND_S;
			_scrSpeed.first = { 0.5 };
			_scrSpeed.second = { 0.75 };
			_fix = { 1080 , 1080 };
			break;
		case stage::number::SN_1:	// ステージ1
			_graphKey = background::BACK_GROUND_1;
			_scrSpeed.first = { STAGE1_BACK_00, STAGE1_BACK_01, STAGE1_BACK_02 };
			_scrSpeed.second = { STAGE1_BACK_00, STAGE1_BACK_01, STAGE1_BACK_02 };
			_fix = { 0, 0};
			break;
		case stage::number::SN_2:	// ステージ2
			_graphKey = background::BACK_GROUND_2;
			_scrSpeed.first = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			_scrSpeed.second = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			_fix = { 0, 0 };
			break;
		case stage::number::SN_B:		// ボスステージ
			_graphKey = background::BACK_GROUND_B;
			_scrSpeed.first = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			_scrSpeed.second = { STAGE2_BACK_00, STAGE2_BACK_01, STAGE2_BACK_02, STAGE2_BACK_03 };
			_fix = { 540, 540 };
			break;
		default:	// 該当なし
#ifdef _DEBUG
			OutputDebugString("BackGround->ChangeGraph(const int stageNo) 対象のステージは存在しません。\n");
#endif
			return;
		}
		Init();	// 初期化をかける
	}

	bool BackGround::IsChanege() {
		auto no = KeyNumber();
		if (_stageNo == no) return false;
		_stageNo = no;
		return true;
	}

	int BackGround::KeyNumber() {
		auto skey = _game.GetModeServer()->GetModeMain()->StageKey();
		if (skey == stage::STAGE_0) return stage::number::SN_S;
		if (skey == stage::STAGE_1) return stage::number::SN_1;
		if (skey == stage::STAGE_2) return stage::number::SN_2;
		if (skey == stage::STAGE_2_1) return stage::number::SN_2;
		if (skey == stage::STAGE_2_2) return stage::number::SN_2;
		if (skey == stage::STAGE_3) return stage::number::SN_B;
		return -1;
	}

	void BackGround::BackDraw() {
		Vector2 xy = _pos;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		auto gh = graph::ResourceServer::GetHandles(_graphKey, 1);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
	}

	void BackGround::BigManage() {
		// ワールド座標の移動量を取得
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		auto moveY = _game.GetMapChips()->BeforeWorldPos().GetY() * -1;
		// auto moveY = _game.GetMapChips()->GetWorldVector().IntY() * -1;


		for (auto i = 0; i < _positions.first.size(); ++i) {
			// ワールドX座標はスクロール開始地点を超えているか？
			if (_game.GetMapChips()->IsScrollX() == true) {
				// 移動量分だけ座標をずらす
				_positions.first[i].GetPX() += moveX * _scrSpeed.first[i];
				_positions.second[i].GetPX() += moveX * _scrSpeed.first[i];
				// 一枚目の修正処理
				if (_positions.first[i].IntX() < -HALF_WINDOW_W) {
					// 漏れた値を算出
					auto vec = -HALF_WINDOW_W - _positions.first[i].GetX();
					_positions.first[i].GetPX() = MAX_SCROLL - vec;
				}
				else if (MAX_SCROLL < _positions.first[i].IntX()) {
					auto vec = _positions.first[i].GetX() - MAX_SCROLL;
					_positions.first[i].GetPX() = -HALF_WINDOW_W + vec;
				}
				// 二枚目の修正処理
				if (_positions.second[i].IntX() < -HALF_WINDOW_W) {
					auto vec = -HALF_WINDOW_W - _positions.second[i].GetX();
					_positions.second[i].GetPX() = MAX_SCROLL - vec;
				}
				else if (WINDOW_W + HALF_WINDOW_W < _positions.second[i].IntX()) {
					auto vec = _positions.second[i].GetX() - MAX_SCROLL;
					_positions.second[i].GetPX() = -HALF_WINDOW_W + vec;
				}
			}

			if (_game.GetMapChips()->IsScrollY() == true) {
				_positions.first[i].GetPY() += moveY * _scrSpeed.second[i];
				_positions.second[i].GetPY() += moveY * _scrSpeed.second[i];

				if (_positions.first[i].GetY() < 0) {
					_positions.first[i].GetPY() = 0;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
				else if (WINDOW_H < _positions.first[i].GetY()) {
					_positions.first[i].GetPY() = WINDOW_H;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
			}
		}
	}

	void BackGround::NormalManage() {
		if (_scroll != true) return;
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;

		for (auto i = 0; i < _positions.first.size(); ++i) {
			// ワールドX座標はスクロール開始地点を超えているか？
			if (_game.GetMapChips()->IsScrollX() == true) {
				// 移動量分だけ座標をずらす
				_positions.first[i].GetPX() += moveX * _scrSpeed.first[i];
				_positions.second[i].GetPX() += moveX * _scrSpeed.first[i];
				// 一枚目の修正処理
				if (_positions.first[i].IntX() < -HALF_WINDOW_W) {
					// 漏れた値を算出
					auto vec = -HALF_WINDOW_W - _positions.first[i].GetX();
					_positions.first[i].GetPX() = MAX_SCROLL - vec;
				}
				else if (MAX_SCROLL < _positions.first[i].IntX()) {
					auto vec = _positions.first[i].GetX() - MAX_SCROLL;
					_positions.first[i].GetPX() = -HALF_WINDOW_W + vec;
				}
				// 二枚目の修正処理
				if (_positions.second[i].IntX() < -HALF_WINDOW_W) {
					auto vec = -HALF_WINDOW_W - _positions.second[i].GetX();
					_positions.second[i].GetPX() = MAX_SCROLL - vec;
				}
				else if (WINDOW_W + HALF_WINDOW_W < _positions.second[i].IntX()) {
					auto vec = _positions.second[i].GetX() - MAX_SCROLL;
					_positions.second[i].GetPX() = -HALF_WINDOW_W + vec;
				}
			}
		}
	}

	void BackGround::ScrollY() {
		for (auto i = 0; i < _positions.first.size(); ++i) {
			if (_game.GetMapChips()->IsScrollY() == true) {
				_positions.first[i].GetPY() = _game.GetMapChips()->GetWorldPosition().GetY();
				_positions.second[i].GetPY() = _game.GetMapChips()->GetWorldPosition().GetY();

				if (_positions.first[i].GetY() < 0) {
					_positions.first[i].GetPY() = 0;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
				else if (WINDOW_H < _positions.first[i].GetY()) {
					_positions.first[i].GetPY() = WINDOW_H;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
			}
		}
	}
}