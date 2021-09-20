#include "TitleLogo.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "ObjectServer.h"
#include "ModeServer.h"
#include "Game.h"
#include <DxLib.h>

namespace {
	constexpr auto PAL_MIN = 0;
	constexpr auto PAL_MAX = 255;
}

namespace inr {

	TitleLogo::TitleLogo(Game& game) : Image(game), _hitCol1(), _hitCol2() {
		Init();
	}

	void TitleLogo::Init() {
		_pos = { 960, 800 };
		_position2 = { 960, 675 };
		_pal = PAL_MIN;
		_graphKey = TITLE_EXIT1;
		_graphKey2 = TITLE_START1;

		_hitCol2 = { _pos, TITLE_START_WIDTH / 4, TITLE_UI_HEIGHT / 4, true };
		_hitCol1 = { _position2, TITLE_EXIT_WIDTH / 2, TITLE_UI_HEIGHT / 4, true };
	}

	void TitleLogo::Process() {
		// 透明度の変更
		if (_pal != PAL_MAX) {
			_pal += 4;
			if (PAL_MAX < _pal) _pal = PAL_MAX;
		}

		auto&& objs = _game.GetObjectServer()->GetObjects();
		for (auto& obj : objs) {
			const auto& collision = obj->GetMainCollision();
			if (_hitCol1.HitCheck(collision)) {
				if (_game.GetTrgKey() == PAD_INPUT_3) {
					// ゲーム本編に遷移する
					auto sound = se::SoundServer::GetSound(system::SOUDN_GAME_START);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(system::SOUDN_GAME_START));

					_game.GetModeServer()->ModeChange(mode::MAIN);
					CollisionOut();
				}
			}
			if (_hitCol2.HitCheck(collision)) {
				if (_game.GetTrgKey() == PAD_INPUT_3) {
					auto sound = se::SoundServer::GetSound(system::SOUND_GAME_END);
					PlaySoundMem(sound, se::SoundServer::GetPlayType(system::SOUND_GAME_END));
					// プログラムを終了する
					_game.GetModeServer()->GameEnd();
					CollisionOut();
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
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);
		DrawRotaGraph(x1, y1, 1.0, 0, gh1, true, false);
		DrawRotaGraph(x2, y2, 1.0, 0, gh2, true, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
		/*
		auto c = DxLib::GetColor(255, 255, 0);
		DebugBox(_hitCol1);
		DebugBox(_hitCol2);
		*/
#endif
	}

#ifdef _DEBUG
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
#endif

	void TitleLogo::CollisionOut() {
		_hitCol1.GetCollisionFlgB() = false;
		_hitCol2.GetCollisionFlgB() = false;
	}
}