#include "Logo.h"
#include "ResourceServer.h"
#include "Game.h"
#include <DxLib.h>

namespace {
	constexpr auto LOGO_FIRST = 0;
	constexpr auto LOGO_SECOND = 1;// SECOND
	constexpr auto LOGO_THIRD = 2; // third
	constexpr auto LOGO_MAX = 3;

	constexpr auto LUMINANCE = 255 / 60;

	constexpr auto ADD_LUMINANCE = true;
	constexpr auto SUB_LUMINANCE = false;

	constexpr auto MAX_LUMINANCE = 255;
	constexpr auto MIN_LUMINANCE = 0;

	constexpr auto WAIT_TIME = 60;	// 待機時間
}

namespace inr {

	Logo::Logo(Game& game) : Image(game) {
		Init();
	}

	void Logo::Init() {
		_number = LOGO_FIRST;
		_calculation = ADD_LUMINANCE;	// 減算処理に切り替え
		ChangeGraph();
	}

	void Logo::Process() {
		// Aボタンの入力があった場合は処理をスキップする
		if (_number != LOGO_THIRD && _game.GetTrgKey() == PAD_INPUT_3) {
			ChangeGraph();	// アニメーションを切り替え
		}
		// 待機時間がある場合は減算処理だけを行い処理から抜ける
		if (_wait) {
			--_wait;
			return;
		}
		// ModeMainで入力処理があった場合、処理をスキップするように変更する
		// ここで変数の値を変更
		++_fCount;
		// 待ち時間がない場合は輝度の値に変更を加えて終了
		switch (_calculation) {
		case ADD_LUMINANCE:
			_pal += LUMINANCE;
			// 条件を満たした場合は60フレームの間更新をかけないようにする
			if (MAX_LUMINANCE <= _pal) {
				_pal = MAX_LUMINANCE;	// 値を修正
				++_number;
				_calculation = SUB_LUMINANCE;
				_wait = WAIT_TIME;
				ChangeGraph();
			}
			break;
		case SUB_LUMINANCE:
			_pal -= LUMINANCE;
			if (_pal <= MIN_LUMINANCE) {
				_calculation = SUB_LUMINANCE;
				_wait = WAIT_TIME;
			}
			break;
		}
	}

	void Logo::Draw() {
		int x = _pos.IntX();
		int y = _pos.IntY();
		int p = static_cast<int>(_pal);
		int grh = graph::ResourceServer::GetHandles(_graphKey, 0);	// 描画するグラフィックハンドルの取得
		SetDrawBlendMode(DX_BLENDMODE_SUB, p);
		DrawGraph(x, y, grh, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// 他の描画に反映されないようにノーマルブレンドに戻す
	}

	void Logo::AnimationInit() {
		_pal = 0;
		_graphKey = TEAM_LOGO;	// チームロゴに切り替え
	}

	void Logo::ChangeGraph() {
		_fCount = 0;
		_pal = 255;		
		// 現在のナンバーに応じてキーを切り替える
		switch (_number) {
		case LOGO_FIRST:	// AMGロゴに遷移
			_graphKey = AMG_LOGO;
			return;
		case LOGO_SECOND:	// チームロゴに遷移
			_graphKey = TEAM_LOGO;
			return;
		case LOGO_THIRD:	// タイトルロゴに遷移
			_graphKey = TITLE_LOGO;
			return;
		case LOGO_MAX:
			return;
		}
	}
}
