/*****************************************************************//**
 * @file   Logo.cpp
 * @brief  ロゴクラス（画像クラスのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Logo.h"
#include "ResourceServer.h"
#include "ObjectServer.h"
#include "Game.h"
#include "SoulCursor.h"
#include <DxLib.h>
#include <memory>

namespace {
	// ロゴ番号
	constexpr auto LOGO_FIRST = 0;				// AMGロゴフェードイン
	constexpr auto LOGO_SECOND = 1;				// AMGロゴフェードアウト
	constexpr auto LOGO_THIRD = 2;				// チームロゴフェードイン
	constexpr auto LOGO_FOURTH = 3;				// チームロゴフェードアウト
	constexpr auto LOGO_MAX = 4;					// タイトルアニメーション

	constexpr auto LUMINANCE = 255 / 60;	
	constexpr auto WAIT_TIME = 120;				// 待機時間
	// 最大フレーム数
	constexpr auto AMG_FADEIN = 60;
	constexpr auto AMG_FADEOUT = 60;
	constexpr auto TEAM_FADEIN = 60;
	constexpr auto TEAM_FADEOUT = 60;
	constexpr auto TITLE_FRAME = 90;
	// 総分割数
	constexpr auto ALLNUM_34 = 34;
	constexpr auto ALLNUM_30 = 30;

	namespace image {
		// アニメーションフラグ
		constexpr auto FADEIN = true;
		constexpr auto FADEOUT = false;
	}
}

namespace inr {

	Logo::Logo(Game& game) : Image(game), _rgb(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND) {
		Init();		// 初期化
	}
	void Logo::Init() {
		// 各種初期化
		_wait = 0;
		_number = LOGO_FIRST;
		_calculation = rgb::ADD;
		_animation = true;
		_tlogo = nullptr;
		ChangeGraph();
	}
	// 更新
	void Logo::Process() {
		// タイトルロゴがnullでは無い場合、更新処理を呼び出し
		if (_tlogo != nullptr) _tlogo->Process();
		// アニメーション
		if (_animation) {
			// Aボタンの入力があった場合は処理をスキップする
			if (_number != LOGO_MAX && (_game.GetTrgKey() == PAD_INPUT_3)) {
				AnimaSkip();		// アニメーションをスキップする
			}
			// 待機時間がある場合は減算処理だけを行い処理から抜ける
			if (_wait) {
				--_wait;				// 減算処理を行う
				if (_wait == 0) {
					_fCount = 1;	// フレームカウンタ
					_rgb.Max();		// RGBを最大にする
				}
				return;
			}
			++_fCount;		// カウンタの加算
			// フェードイン・フェードアウトのどちらの処理を行うか
			switch(_calculation) {
			case true:		// フェードアウト
				// カウンタが最大フレーム未満の場合
				if (_fCount > _maxFrame) {
					switch (_number) {
					case LOGO_MAX:	// タイトルアニメーションの場合
						_animation = false;				// アニメーション終了
						_maxFrame = TITLE_FRAME;	// 最大フレーム設定
						return;
					default:
						++_number;								// 処理番号加算
						_calculation = false;			// フェードインに遷移
						_wait = WAIT_TIME;				// 待機時間設定
						ChangeGraph();						// ロゴ画像切り替え
						return;
					}
				}
				// カウンタが最大かつ処理番号が最大の場合
				if (_number == LOGO_MAX) {
					_rgb.Update(true, LUMINANCE);	// RGBを更新する
					break;
				}
			case false:		// フェードイン
				// カウンタが最大フレーム未満の場合
				if (_fCount > _maxFrame) {
					switch (_number) {
					case LOGO_FOURTH:	// チームロゴフェードアウトの場合
						_number = LOGO_MAX;		// 処理番号を最大にする
						_calculation = true;	// フェードイン開始
						ChangeGraph();				// ロゴ画像切り替え
						return;
					default:
						++_number;						// 処理番号加算
						_calculation = true;	// フェードインに遷移
						_wait = 10;						// 待機時間設定
						ChangeGraph();			// ロゴ画像切り替え
						return;
					}
				}
			}
			return;
		}
		++_fCount;	// カウンタ加算
		if (_maxFrame < _fCount) _fCount = 1;	// 最大値を超えた場合は修正する
	}

	void Logo::Draw() {
		// 描画座標
		int x = _pos.IntX();
		int y = _pos.IntY();
		// グラフィックハンドルの取得
		auto interval = (static_cast<double>(_allnum) - 1) / (static_cast<double>(_maxFrame));
		int no = static_cast<int>(_fCount * interval);
		int grh = graph::ResourceServer::GetHandles(_graphKey, no);
		SetDrawBright(_rgb.Red(), _rgb.Green(), _rgb.Blue());						// 輝度設定
		DrawRotaGraph(x, y, 1.0, 0, grh, true, false);									// 描画
		SetDrawBright(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND);	// 輝度初期化
		if (_tlogo != nullptr) _tlogo->Draw();													// ポインタがある場合は描画処理呼び出し
	}

	void Logo::AnimationInit() {
		_graphKey = TEAM_LOGO;		// チームロゴに切り替え
	}

	void Logo::ChangeGraph() {
		_fCount = 1;	// カウンタ初期化
		// 現在の処理番号に応じてキーおよび値を切り替える
		switch (_number) {
		case LOGO_FIRST:	// AMGロゴフェードイン
			_allnum = ALLNUM_34;
			_graphKey = titles::TITLE_ALI;
			_maxFrame = AMG_FADEIN;
			return;
		case LOGO_SECOND:	// AMGロゴフェードアウト
			_allnum = ALLNUM_34;
			_graphKey = titles::TITLE_ALO;
			_maxFrame = AMG_FADEOUT;
			return;
		case LOGO_THIRD:	// チームロゴフェードイン
			_rgb.Min();
			_allnum = ALLNUM_30;
			_graphKey = titles::TITLE_TLI;
			_maxFrame = TEAM_FADEIN;
			return;
		case LOGO_FOURTH:	// チームロゴフェードアウト
			_graphKey = titles::TITLE_TLO;
			_maxFrame = TEAM_FADEOUT;
			return;
		case LOGO_MAX:		// タイトルロゴ
			_rgb.Min();
			_graphKey = titles::TITLE_T;
			_maxFrame = 60;
			// タイトルロゴの生成および登録
			_tlogo = std::make_unique<TitleLogo>(_game.GetGame());
			_game.GetObjectServer()->Add(std::move(std::make_unique<SoulCursor>(_game.GetGame())));
			return;
		}
	}

	void Logo::AnimaSkip() {
		_wait = 0;						// 待ち時間の初期化
		_calculation = true;	// フェードアウトに遷移
		// 現在の処理番号に応じて処理のスキップを行う
		switch (_number) {
		case LOGO_FIRST:
			_wait = 10;
			_number = LOGO_THIRD;
			break;
		case LOGO_SECOND:
			_wait = 10;
			_number = LOGO_THIRD;
			break;
		case LOGO_THIRD:
			_number = LOGO_MAX;
			break;
		case LOGO_FOURTH:
			_number = LOGO_MAX;
			break;
		}
		ChangeGraph();				// ロゴ画像の切り替え
	}
}
