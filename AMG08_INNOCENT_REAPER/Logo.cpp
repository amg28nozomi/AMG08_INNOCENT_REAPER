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

	constexpr auto WAIT_TIME = 120;	// 待機時間
}

namespace inr {

	Logo::Logo(Game& game) : Image(game), _rgb() {
		Init();
	}

	void Logo::Init() {
		_wait = 0;
		_number = LOGO_FIRST;
		_calculation = rgb::ADD;	// 減算処理に切り替え
		ChangeGraph();
	}

	void Logo::Process() {
		if (_number < LOGO_MAX) {
			// Aボタンの入力があった場合は処理をスキップする
			if (_number != LOGO_THIRD && _game.GetTrgKey() == PAD_INPUT_3) {
				++_number;
				_calculation = rgb::ADD;
				ChangeGraph();	// アニメーションを切り替え
			}
			// 待機時間がある場合は減算処理だけを行い処理から抜ける
			if (_wait) {
				--_wait;
				if (_wait == 0) _fCount = 0;
				return;
			}
			// ModeMainで入力処理があった場合、処理をスキップするように変更する
			// ここで変数の値を変更
			++_fCount;
			// 待ち時間がない場合は輝度の値に変更を加えて終了
			switch (_calculation) {
			case rgb::ADD:
				// 条件を満たした場合は60フレームの間更新をかけないようにする
				if (_fCount == 60) {
					_calculation = rgb::SUB;
					_wait = WAIT_TIME;
					return;
				}
				_rgb.Update(_calculation, LUMINANCE);
				break;
			case rgb::SUB:
				if (_fCount == 60) {
					++_number;
					_calculation = rgb::ADD;
					ChangeGraph();
					return;
				}
				_rgb.Update(_calculation, LUMINANCE);
				break;
			}
		}
	}

	void Logo::Draw() {
		int x = _pos.IntX();
		int y = _pos.IntY();
		int grh = graph::ResourceServer::GetHandles(_graphKey, 0);	// 描画するグラフィックハンドルの取得
		SetDrawBright(_rgb.Red(), _rgb.Green(), _rgb.Blue());
		DrawRotaGraph(x, y, 1.0, 0, grh, true, false);
		SetDrawBright(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND);
	}

	void Logo::AnimationInit() {
		_graphKey = TEAM_LOGO;	// チームロゴに切り替え
	}

	void Logo::ChangeGraph() {
		_fCount = 0;
		_rgb.Min();
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
