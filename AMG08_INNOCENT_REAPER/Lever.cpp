#include "Lever.h"
#include "Game.h"
#include "GimmickServer.h"
#include "SoundServer.h"
#include "Door.h"
#include "ModeServer.h"
#include "ModeMain.h"

namespace inr {

	Lever::Lever(Game& game) : GimmickBase(game) {
		_gType = GimmickType::LEVER;
		_door.reset();
		_divKey = { gimmick::lever::KEY_LEVER, "" };
		_motionKey = { 
			{ _divKey.first, {20, 50}},
			{ gimmick::lever::KEY_LEVER_BOSS, {30, 50}}
		};
	}

	void Lever::Init() {
	}

	void Lever::Process() {
		if (IsAnimationMax()) return;
		if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
			if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) ++_aCount;
			return;
		}
		else if (_door->IsSwitch()) ++_aCount;
	}

	void Lever::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		int graph;
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, true);
#ifdef _DEBUG
		DrawDebugBox(_mainCollision);
#endif
	}

	void Lever::OpenDoor() {
		if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
			if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) return;
		}
		else if (_door->IsSwitch() == gimmick::ON ) return;	// ドアがオープンの場合は処理を抜ける
		auto sh = SoundResearch(gimmick::lever::KEY_LEVER);
		PlaySoundMem(sh, se::SoundServer::GetPlayType(_divKey.second));
		if (_oValue.GimmickType() == gimmick::door::D_BOSS) { 
			// UIの描画を行う
			_game.GetModeServer()->GetModeMain()->OpenBossStage();
		}	// ボスステージへと続く扉を解放する
		else _door->SwitchOn();	// スイッチオン
#ifdef _DEBUG
		_mainCollision.SetDrawFlag() = false;
#endif
	}

	void Lever::SetParameter(ObjectValue objValue) {
		_oValue = objValue;

		_position = _oValue.Positions()[0];
		_mainCollision = {  _position, 30, 30, 50, 70, true  };	// 当たり判定の設定
		// ボス専用だった場合、専用処理を行う
		if (_oValue.GimmickType() == gimmick::door::D_BOSS) {
			_divKey.first = gimmick::lever::KEY_LEVER_BOSS;	// グラフィック切り替え
			if (_game.GetModeServer()->GetModeMain()->BossOpen() == true) {
				_aCount = AnimationCountMax();
#ifdef _DEBUG
				_mainCollision.SetDrawFlag() = false;
#endif
			}
			return;
		}

		auto gdoor = std::make_shared<Door>(_game.GetGame());
		std::string gh;
		switch (_oValue.GimmickType()) {
		case gimmick::door::D_LEVER:
			gh = gimmick::door::KEY_DOOR_LEVER;
			break;
		case gimmick::door::D_RED:
			gh = gimmick::door::D_RED;
			break;
		case gimmick::door::D_BLUE:
			gh = gimmick::door::D_BLUE;
			break;
		default:
			return;
		}

		if (_oValue.GimmickFlag() == oscenario::gimmick::FLAG_TRUE) {
			_aCount = AnimationCountMax();
#ifdef _DEBUG
			_mainCollision.SetDrawFlag() = false;
#endif
		}

		gdoor->SetParameter(_oValue.Positions()[1], gh, _oValue.GimmickFlag());	// 扉の生成
		_door = gdoor;
		_game.GetGimmickServer()->Add(gdoor);
	}

	void Lever::ObjValueUpdate() {
		if (_oValue.GimmickType() == gimmick::door::D_BOSS) return;	// 処理を行わない
		auto flag = oscenario::gimmick::FLAG_FALSE;
		if (_door->IsSwitch() == gimmick::ON) flag = oscenario::gimmick::FLAG_TRUE;
		_oValue.FlagUpdate(flag);	// 更新を行う
	}
}