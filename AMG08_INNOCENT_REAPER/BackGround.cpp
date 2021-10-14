/*****************************************************************//**
 * @file   BackGround.cpp
 * @brief  背景クラス（画像クラスのサブクラス）
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "BackGround.h"
#include "Vector2.h"
#include "ResourceServer.h"
#include "Game.h"
#include "MapChips.h"
#include "ModeServer.h"
#include "ModeMain.h"
#include <DxLib.h>

namespace {
	// スクロール速度
	constexpr auto BACKGROUND_SPEED = 0.5;
	// スクロール上限
	constexpr auto MAX_SCROLL = 1920 + 960;
	// スクロール下限
	constexpr auto MIN_SCROLL = -960;
	// ステージ1の各種スクロール速度
	constexpr auto STAGE1_BACK_00 = 0.30;
	constexpr auto STAGE1_BACK_01 = 0.4;
	constexpr auto STAGE1_BACK_02 = 0.6;
	// ステージ2の各種スクロール速度
	constexpr auto STAGE2_BACK_00 = 0.25;
	constexpr auto STAGE2_BACK_01 = 0.40;
	constexpr auto STAGE2_BACK_02 = 0.50;
	constexpr auto STAGE2_BACK_03 = 0.60;
}

namespace inr {
	// コンストラクタ
	BackGround::BackGround(Game& game) : Image(game){
		// 各種変数初期化
		_stageNo = stage::number::SN_NULL;
		_pos = { 960 , 1080 };
		_fix = { 0, 0 };
		_scroll = true;	// スクロールオン
	}
	// 初期化
	void BackGround::Init() {
		// 背景画像枚数分、初期化を行う
		for (int n = 0; n < static_cast<int>(_scrSpeed.first.size()); ++n) {
			_positions.first.emplace_back(HALF_WINDOW_W, _fix.first);
			_positions.second.emplace_back(MAX_SCROLL, _fix.second);
		}
		ScrollY();	// 修正をかける
	}
	// 更新
	void BackGround::Process() {
		ChangeGraph();	// 背景画像を切り替えるか
		// 画像キーに応じて呼び出すスクロール処理を変更
		if (_graphKey == background::BACK_GROUND_B) NormalManage();
		else BigManage();
	}
	// 描画
	void BackGround::Draw() {
		// 背景画像枚数分の描画処理を実行する
		for (auto number = 0; number < static_cast<int>(_positions.first.size()); ++number) {
			auto x1 = _positions.first[number].IntX();
			auto y1 = _positions.first[number].IntY();

			auto x2 = _positions.second[number].IntX();
			auto y2 = _positions.second[number].IntY();
			auto gh = graph::ResourceServer::GetHandles(_graphKey, number);
			DrawRotaGraph(x1, y1, 1.0, 0, gh, true, false);
			DrawRotaGraph(x2, y2, 1.0, 0, gh, true, false);
		}
		// 現座のステージが
		if (_game.GetModeServer()->GetModeMain()->StageKey() == stage::STAGE_0) BackDraw();
	}
	// ステージに応じた画像に切り替える（引数:現在のステージ）
	void BackGround::ChangeGraph() {
		// ステージは切り替わったか？
		if (IsChanege() != true) return;
		// 切り替わった場合は各種変数の要素を解放する
		_graphKey.clear();
		_scrSpeed.first.clear();
		_scrSpeed.second.clear();
		_positions.first.clear();
		_positions.second.clear();
		// 現在のステージに応じて各種値を登録
		switch (_stageNo) {
		case stage::number::SN_S:	// ステージS
			_graphKey = background::BACK_GROUND_S;
			_scrSpeed.first = { 0.5, 0.35 };
			_scrSpeed.second = { 0.75, 0.75 };
			_fix = { 1080 , 1080 };
			break;
		case stage::number::SN_T:	// チュートリアル
			_graphKey = background::BACK_GROUND_S;
			_scrSpeed.first = { 0.5 };
			_scrSpeed.second = { 0.75 };
			_fix = { 0, 0 };
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
	// キーは切り替わったか？
	bool BackGround::IsChanege() {
		auto no = KeyNumber();	// 画像番号を取得
		// 番号が等しい場合は処理を中断
		if (_stageNo == no) return false;
		// 違う場合は更新
		_stageNo = no;
		return true;
	}
	// ステージ番号の判定
	int BackGround::KeyNumber() {
		// ステージキーを取得
		auto skey = _game.GetModeServer()->GetModeMain()->StageKey();
		if (skey == stage::STAGE_0) return stage::number::SN_S;
		if (skey == stage::STAGE_T) return stage::number::SN_T;
		if (skey == stage::STAGE_1) return stage::number::SN_1;
		if (skey == stage::STAGE_2) return stage::number::SN_2;
		if (skey == stage::STAGE_2_1) return stage::number::SN_2;
		if (skey == stage::STAGE_2_2) return stage::number::SN_2;
		if (skey == stage::STAGE_3) return stage::number::SN_B;
		return -1;
	}
	// 前描画
	void BackGround::BackDraw() {
		Vector2 xy = _pos;	// 現在の座標
		_game.GetMapChips()->Clamp(xy);	// スクリーン座標にクランプする
		// 描画座標の算出
		auto x = xy.IntX();
		auto y = xy.IntY();
		// グラフィックハンドルの取得
		auto gh = graph::ResourceServer::GetHandles(background::ALTAR);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);	// 描画
	}
	// スクロール処理(大)
	void BackGround::BigManage() {
		// ワールド座標の移動量を取得
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		auto moveY = _game.GetMapChips()->BeforeWorldPos().GetY() * -1;
		// 画像枚数分処理を行う
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
			// Y座標の修正
			if (_game.GetMapChips()->IsScrollY() == true) {
				// 移動量分だけ座標を移動させる
				_positions.first[i].GetPY() += moveY * _scrSpeed.second[i];
				_positions.second[i].GetPY() += moveY * _scrSpeed.second[i];
				// 下限を下回った場合は値を修正
				if (_positions.first[i].GetY() < 0) {
					_positions.first[i].GetPY() = 0;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
				// 上限を上回った場合は値を修正
				else if (WINDOW_H < _positions.first[i].GetY()) {
					_positions.first[i].GetPY() = WINDOW_H;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
			}
		}
	}
	// スクロール処理(小)
	void BackGround::NormalManage() {
		// スクロール処理がオフの場合、処理を行わない
		if (_scroll != true) return;
		// 移動量Xの取得
		auto moveX = _game.GetMapChips()->BeforeWorldPos().IntX() * -1;
		// 画像枚数分処理を行う
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
				} else if (MAX_SCROLL < _positions.first[i].IntX()) {
					auto vec = _positions.first[i].GetX() - MAX_SCROLL;
					_positions.first[i].GetPX() = -HALF_WINDOW_W + vec;
				}
				// 二枚目の修正処理
				if (_positions.second[i].IntX() < -HALF_WINDOW_W) {
					auto vec = -HALF_WINDOW_W - _positions.second[i].GetX();
					_positions.second[i].GetPX() = MAX_SCROLL - vec;
				} else if (WINDOW_W + HALF_WINDOW_W < _positions.second[i].IntX()) {
					auto vec = _positions.second[i].GetX() - MAX_SCROLL;
					_positions.second[i].GetPX() = -HALF_WINDOW_W + vec;
				}
			}
		}
	}
	// Y座標の修正
	void BackGround::ScrollY() {
		// Y座標の修正
		for (auto i = 0; i < _positions.first.size(); ++i) {
			// スクロール座標を超えているか？
			if (_game.GetMapChips()->IsScrollY() == true) {
				// 座標を代入
				_positions.first[i].GetPY() = _game.GetMapChips()->GetWorldPosition().GetY();
				_positions.second[i].GetPY() = _game.GetMapChips()->GetWorldPosition().GetY();
				// 下限を下回った場合は修正
				if (_positions.first[i].GetY() < 0) {
					_positions.first[i].GetPY() = 0;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
				// 上限を上回った場合も修正
				else if (WINDOW_H < _positions.first[i].GetY()) {
					_positions.first[i].GetPY() = WINDOW_H;
					_positions.second[i].GetPY() = _positions.first[i].GetY();
				}
			}
		}
	}
}