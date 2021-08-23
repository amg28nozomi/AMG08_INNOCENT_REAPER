#include "TitleLogo.h"
#include "ResourceServer.h"
#include "ObjectServer.h"
#include "ModeServer.h"
#include "Game.h"
#include <DxLib.h>

namespace inr {

	TitleLogo::TitleLogo(Game& game) : Image(game), _rgb(), _hitCol1(), _hitCol2() {
		Init();
	}

	void TitleLogo::Init() {
		_pos = { 510, 700 };
		_position2 = { 1410, 700 };
		_graphKey = TITLE_EXIT1;
		_graphKey2 = TITLE_START1;

		_hitCol2 = { _pos, TITLE_START_WIDTH / 2, TITLE_UI_HEIGHT / 2, true };
		_hitCol1 = { _position2, TITLE_EXIT_WIDTH / 2, TITLE_UI_HEIGHT / 2, true };
	}

	void TitleLogo::Process() {
		auto&& objs = _game.GetObjectServer()->GetObjects();
		for (auto& obj : objs) {
			const auto& collision = obj->GetMainCollision();
			if (_hitCol1.HitCheck(collision)) {
				if (_game.GetTrgKey() == PAD_INPUT_3) {
					// ゲーム本編に遷移する
					_game.GetObjectServer()->ObjectsClear();
					_game.GetModeServer()->ModeChange(mode::MAIN);
				}
			}
			if (_hitCol2.HitCheck(collision)) {
				if (_game.GetTrgKey() == PAD_INPUT_3) {
					// プログラムを終了する
					_game.ProgramEnd();
				}
			}
		}
	}

	void TitleLogo::Draw() {
		auto x1 = _pos.IntX();
		auto y1 = _pos.IntY();
		auto x2 = _position2.IntX();
		auto y2 = _position2.IntY();
		auto gh1 = graph::ResourceServer::GetHandles(_graphKey, 0);	// 描画するグラフィックハンドルの取得
		auto gh2 = graph::ResourceServer::GetHandles(_graphKey2, 0);
		SetDrawBright(_rgb.Red(), _rgb.Green(), _rgb.Blue());
		DrawRotaGraph(x1, y1, 1.0, 0, gh1, true, false);
		DrawRotaGraph(x2, y2, 1.0, 0, gh2, true, false);
		SetDrawBright(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND);

#ifdef _DEBUG
		/*auto c = DxLib::GetColor(255, 255, 0);
		DebugBox(_hitCol1);
		DebugBox(_hitCol2);*/
#endif
	}

	void TitleLogo::DebugBox(Collision col) {
		auto drawFlg = col.GetbDrawFlg();
		// 描画判定はオンになっているかどうか？
		if (drawFlg) {
			auto min = col.GetMin();
			auto max = col.GetMax();

			auto minx = min.IntX();
			auto maxx = max.IntX();
			auto miny = min.IntY();
			auto maxy = max.IntY();
			DxLib::DrawBox(minx, miny, maxx, maxy, GetColor(255, 255, 0), FALSE);
		}
	}
}