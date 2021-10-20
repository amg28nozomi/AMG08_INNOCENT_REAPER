/*****************************************************************//**
 * @file   FadeBlack.cpp
 * @brief  フェードイン・フェードアウトを行うフェードブラッククラス（画像クラスのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "FadeBlack.h"
#include "ResourceServer.h"
#include <DxLib.h>

namespace {
	constexpr auto LUMINANCE = 255 / 60;
	constexpr auto FADE_VALUE = 255 / 60;
}

namespace inr {

	FadeBlack::FadeBlack(Game& game) : Image(game) {
		_pal = 0;
		_type = image::FADE_OUT;
		Init();
	}

	FadeBlack::~FadeBlack() {
	}

	void FadeBlack::Init() {
		// 各種初期化
		_end = true;
		_interval = 0;
		_isInterval = false;
		_addEnd = true;
	}

	void FadeBlack::Process() {
		if (_end == true) return;							// 処理がない場合は更新終了
		if (_addEnd == true) _addEnd = false;	// フラグがある場合は偽にする
		// 猶予時間判定がある場合
		if (_isInterval) {
			// 猶予時間が0になった場合
			if (_interval == 0) {
				_isInterval = false;				// フラグを終了する
				_type = image::FADE_IN;			// フェードインを開始する
				return;
			}
			--_interval;		// 猶予時間を減らす
			return;
		}
		// フラグに応じて処理を行う
		switch (_type) {
		case image::FADE_IN:
			FadeIn();			// フェードイン
			return;
		case image::FADE_OUT:
			FadeOut();		// フェードアウト
			return;
		}
	}

	void FadeBlack::Draw() {
		if (_end == true) return;	// 処理は行わない
		// 画像の取得
		auto graph = graph::ResourceServer::GetHandles(image::BLACK, 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);	
		DrawRotaGraph(961, 540, 1.0, 0, graph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void FadeBlack::FlagChange(bool fadeType, int interval) { 
		// 各種代入
		_type = fadeType;
		_end = false;
		_interval = interval;
	}

	void FadeBlack::FadeIn() {
		_pal -= FADE_VALUE;		// 透明度の更新
		FadeEnd();						// 処理を終了するのかの判定
	}

	void FadeBlack::FadeOut() {
		_pal += FADE_VALUE;		// 透明の更新
		FadeEnd();						// 処理を終了するかの判定
	}

	bool FadeBlack::FadeEnd() {
		// どちらの処理を行っているか
		switch (_type) {
			// フェードイン
		case image::FADE_IN:
			if (0 < _pal) return false;		// 透明度が下限になっていない
			if (_pal < 0) _pal = 0;				// 下限未満になった場合は値を修正する
			_end = true;									// 処理終了
			_isInterval = false;					// 猶予フラグをオフにする
			return true;
			// フェードアウト
		case image::FADE_OUT:
			if (_pal < 255) return false;	// 透明度が上限になっていない
			if (255 < _pal) _pal = 255;		// 上限を超過した場合は値を修正する
			_addEnd = true;								// 加算処理終了
			break;
		}
		_isInterval = true;		// 猶予処理を行う
		return true;
	}
}