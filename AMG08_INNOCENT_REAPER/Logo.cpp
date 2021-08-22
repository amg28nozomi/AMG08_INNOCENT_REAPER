#include "Logo.h"
#include "ResourceServer.h"
#include "Game.h"
#include <DxLib.h>

namespace {
	constexpr auto LOGO_FIRST = 0;
	constexpr auto LUMINANCE = 255 / 60;
}

namespace inr {

	Logo::Logo(Game& game) : Image(game) {
		Init();
	}

	void Logo::Init() {
		_graphKey = AMG_LOGO;
		_pal = 0;
		_number = 0;
	}

	void Logo::Process() {
		// ModeMainで入力処理があった場合、処理をスキップするように変更する
		// ここで変数の値を変更
		
		// Aボタンの入力があった場合は処理をスキップする
		if (_game.GetTrgKey() == PAD_INPUT_3) {
			AnimationInit();	// アニメーションを変更する
		}
	}

	void Logo::Draw() {
		int x = _pos.IntX();
		int y = _pos.IntY();
		int grh = graph::ResourceServer::GetHandles(_graphKey, 0);	// 描画するグラフィックハンドルの取得
		// SetDrawBlendMode(DX_BLENDMODE_SUB, _pal);
		DrawGraph(x, y, grh, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// 他の描画に反映されないようにノーマルブレンドに戻す
	}

	void Logo::AnimationInit() {
		_pal = 0;
		_graphKey = TEAM_LOGO;	// チームロゴに切り替え
	}

	void Logo::ChangeGraph() {
		_pal = 0;
		
		switch (_number) {

		}
	}
}
