/*****************************************************************//**
 * @file   Item.cpp
 * @brief  アイテムクラス
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Item.h"
#include "Game.h"
#include "MapChips.h"
#include "Particle_Image.h"
#include "ObjectServer.h"
#include "Player.h"
#include "ObjectValue.h"

#include "ModeServer.h"
#include "ModeMain.h"
#include "ImageServer.h"
#include "SoundServer.h"

namespace {
	constexpr auto ITEM_MAX = 28 * 2;	// 最大フレーム数
}

namespace inr {
	// コンストラクタ
	Item::Item(Game& game) : _game(game), _oValue() {
		// 各種初期化
		_gkey = "";
		_position = { 0, 0 };
		_col = { _position, 0, 0, true };
		_count = 0;
		_maxFrame = ITEM_MAX;
		_del = false;
		_isGet = false;
	}
	// 初期化
	void Item::Init() {
	}
	// 更新
	void Item::Process() {
		if (_isGet == true) return;			// 既に入手されている場合は処理を行わない
		// カウンタの制御
		if (_count < _maxFrame) ++_count;	// 最大フレーム未満の場合はカウンタを加算
		else _count = 0;					// 上限に到達した場合は初期化

		auto pbox = _game.GetObjectServer()->GetPlayer()->GetMainCollision();	// 自機の当たり判定を取得
		if (_col.HitCheck(pbox) == true) {
			// 接触状態かつ、ボタン入力があった場合はmessageを表示する
			if ((_game.GetTrgKey() == PAD_INPUT_4) == true) {
				_game.GetModeServer()->GetModeMain()->GetItemImages()->ImageChange(_messageNo);
				_isGet = true;	// 入手した
			}
		}
	}
	// 描画
	void Item::Draw() {
		if (_isGet == true) return;		// 入手されている場合は描画しない
		// 描画座標の算出
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// 描画画像の算出
		auto allnum = graph::ResourceServer::GetAllNum(_gkey);
		auto interval = _maxFrame / allnum;		// 猶予時間の割り出し
		auto no = _count / interval % allnum;	// 描画する画像の算出
		auto gh = graph::ResourceServer::GetHandles(_gkey, no);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
	}
	// オブジェクト情報の登録
	void Item::SetParameter(ObjectValue ovalue) {
		_oValue = ovalue;	// 情報登録
		// 各種初期化
		_gkey = item::ITEM;
		_isGet = ovalue.GimmickFlag() == TRUE;
		_messageNo = ovalue.GimmickType();
		_position = ovalue.Positions().front();
		_col = { _position, 50, 50, true };
	}
	// オブジェクト情報の更新
	void Item::ObjValueUpdate() {
		if (_oValue.GimmickFlag() == TRUE) return;	// 既にフラグオンの場合は更新を行わない
		if (_isGet == true) _oValue.FlagUpdate(TRUE);	// フラグを切り替える
	}
}
