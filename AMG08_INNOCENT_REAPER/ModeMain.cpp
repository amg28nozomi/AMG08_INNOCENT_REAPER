#include "ModeMain.h"
#include "SoundServer.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "SoulSkin.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include "FadeBlack.h"
#include "Scenario.h"
#include "Loads.h"
#include "ModeServer.h"
#include "StageTransition.h"
#include "Pause.h"

#include <memory>
#include <unordered_map>

namespace {
	constexpr auto WINDOW_MIN = 0;
}

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_stageKey = stage::CHANGE_NULL;
		_changeKey = stage::CHANGE_NULL;

		_bg = std::make_unique<BackGround>(_game.GetGame());
		_uiSoul = std::make_unique<UI>(_game.GetGame());
		_pause = std::make_unique<Pause>(_game.GetGame());

		/*Scenario::ObjectData stage1;
		stage1.emplace(objtype::PLAYER, {150, 1900});
		stage1.emplace(objtype::SOLDIER_DOLL, { 500, 0 });
		};*/
	}

	ModeMain::~ModeMain() {

	}

	void ModeMain::Init() {
		if (_resetFlg) {
			// 各種オブジェクトをサーバに登録する
			// オブジェクトサーバにプレイヤーを登録
			_stageKey = stage::STAGE_2_1;
			_changeKey = stage::CHANGE_NULL;
			BgmManage(_stageKey);
			_worldPosition = { 1920 / 2, 1080 / 2 };

			TimeClear();
			_game.GetMapChips()->ChangeMap(_stageKey);
			_game.GetScenario()->AddObjects(_stageKey);
			_uiSoul->PlayerUpdate();
			_resetFlg = false;
		}
	}

	void ModeMain::Process() {
		IsStageChange();
		++_modeFrame;

		if (_pause->IsActive() != true) {
			_bg->Process();
			_game.GetMapChips()->Process();
			_game.GetObjectServer()->Process();
			_uiSoul->Process();
			return;
		}
		_pause->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();
		_game.GetMapChips()->Draw();
		_game.GetObjectServer()->Draw();
		_uiSoul->Draw();
		if(_pause->Active() == true) _pause->Draw();
	}

	void ModeMain::ChangeKey(const std::string nextStage) { 
		if (_game.GetModeServer()->IsFadeEnd() == false) return;
		_changeKey = nextStage; 
		_game.GetModeServer()->FadeOut();	// 暗転処理を行う
	}


	bool ModeMain::IsStageChange() {
		// キーは切り替わっているか？
		if (_changeKey == stage::CHANGE_NULL) return false;
		if (_game.GetModeServer()->PalChange() == true) {
			// ギミックの状態を更新する
			BgmManage(_changeKey);	// bgm切り替え
			_game.GetScenario()->ScenarioUpdate(_stageKey);	// 元いた情報に更新をかける
			_game.GetMapChips()->ChangeMap(_changeKey);
			_game.GetObjectServer()->ObjectsClear();
			_game.GetObjectServer()->GetPlayer()->SetParameter(_game.GetMapChips()->GetStageTransition()->SetPosition());	// 自機の座標を更新する
			_game.GetScenario()->AddObjects(_changeKey);
			_stageKey = _changeKey;	// キーを更新する
			_changeKey = stage::CHANGE_NULL;
			return true;
		}
		// 現在の登録されているオブジェクトの削除を行う
		
		// 自機(+保持している魂)、
	}

	bool ModeMain::IsKeyNull() {
		bool flag = _changeKey == stage::CHANGE_NULL;
		return flag;
	}

	bool ModeMain::StageChange() {
		// ステージの切り替え
		return false;
	}

	void ModeMain::SetObjects() {

	}

	bool ModeMain::BgmManage(std::string nextStage) {
		auto bgm = BgmKey(nextStage);	// キー取得
		if (_bgmKey == bgm)	return false;	// キーが等しい場合はBGMを切り替えず鳴らし続ける

		StopSoundMem(se::SoundServer::GetSound(_bgmKey));	// 現在のキーを止める
		_bgmKey = bgm;	// 違う場合はbgmを切り替える
		PlaySoundMem(se::SoundServer::GetSound(_bgmKey), se::SoundServer::GetPlayType(_bgmKey));
		return true;
	}

	std::string ModeMain::BgmKey(std::string key) {
		if (key == stage::STAGE_0) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_1) return bgm::SOUND_STAGE_1;
		if (key == stage::STAGE_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_1) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_3) return bgm::SOUND_STAGE_3;
		else return "";	// 登録されていない
	}
}
