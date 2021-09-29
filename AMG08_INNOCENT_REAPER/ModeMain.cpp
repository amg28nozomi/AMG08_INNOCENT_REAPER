#include "ModeMain.h"
#include "SoundServer.h"
#include "Player.h"
#include "SoldierDoll.h"
#include "SoulSkin.h"
#include "Game.h"
#include "ObjectServer.h"
#include "GimmickServer.h"
#include "ObjectBase.h"
#include "BackGround.h"
#include "FadeBlack.h"
#include "Scenario.h"
#include "Loads.h"
#include "StageUi.h"
#include "ModeServer.h"
#include "StageTransition.h"
#include "Pause.h"
#include "EffectServer.h"
#include "CrowDoll.h"
#include "ForeGround.h"
#include "ItemServer.h"

#include <memory>
#include <unordered_map>

namespace {
	constexpr auto WINDOW_MIN = 0;
}

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_bossBattle = false;
		_stageKey = stage::CHANGE_NULL;
		_changeKey = stage::CHANGE_NULL;

		_bg = std::make_unique<BackGround>(_game.GetGame());
		_stageUi = std::make_unique<StageUi>(_game.GetGame());
		// _uiSoul = std::make_unique<UI>(_game.GetGame());
		_pause = std::make_unique<Pause>(_game.GetGame());
		_eServer = std::make_shared<EffectServer>(_game.GetGame());
		_messageServer = std::make_unique<ImageServer>(_game.GetGame());
		_fg = std::make_unique<ForeGround>(_game.GetGame());
		_itemServer = std::make_unique<ItemServer>();
	}

	ModeMain::~ModeMain() {

	}

	void ModeMain::Init() {
		if (_resetFlg) {
			// 各種オブジェクトをサーバに登録する
			// オブジェクトサーバにプレイヤーを登録
			_stageKey = stage::STAGE_2_2;
			_changeKey = stage::CHANGE_NULL;
			BgmManage(_stageKey);
			_worldPosition = { 1920 / 2, 1080 / 2 };

			TimeClear();
			_fg->SetKey(_stageKey);
			_game.GetMapChips()->ChangeMap(_stageKey);
			_game.GetScenario()->AddObjects(_stageKey);
			_eServer->Init();
			// _uiSoul->PlayerUpdate();
			_stageUi->ChangeNumber(_stageKey);
			//_itemImages->AddImage()
			_bossOpen = false;	// ボスの扉
			_bossBattle = false;
			_resetFlg = false;
		} else {
			_game.GetScenario()->Init();
			_eServer->Init();	// 各種エフェクトを消去する
			_game.GetObjectServer()->AllClear();
			_game.GetGimmickServer()->Clear();
			_pause->Init();
			_messageServer->Init();
			_bg->ChangeGraph();
			// _uiSoul->Reset();
			_resetFlg = true;
		}
	}

	void ModeMain::Process() {
		IsStageChange();	// ステージを切り替えるか？
		StageReset();	// ステージを初期化するか？
		if (_stageUi->FadeDraw() != true && _game.GetModeServer()->IsFadeEnd() == true) _stageUi->DrawStart();
		++_modeFrame;

		if (_messageServer->IsActive() == true) {
			_messageServer->Process();
			return;
		} else if (_pause->IsActive() == true) {
			_pause->Process();	// ポーズ画面更新処理
			return;
		}
		// どのフラグもオンになっていない場合のみ処理を実行する
		_bg->Process();
		_game.GetGimmickServer()->Process();
		_game.GetMapChips()->Process();
		_eServer->Process();
		_game.GetObjectServer()->Process();
		_itemServer->Process();
		_fg->Process();
		// _uiSoul->Process();
		_stageUi->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();	// 背景
		_game.GetGimmickServer()->Draw();	// ギミック
		_game.GetMapChips()->Draw();	// マップチップ
		_eServer->DrawBack();	// エフェクト(後)
		_game.GetObjectServer()->Draw();	// オブジェクト
		_itemServer->Draw();
		_eServer->DrawFormer();	// エフェクト(前)
		_fg->Draw();
		// _uiSoul->Draw();	// HP(UI)
		_stageUi->Draw();	// ステージUI
		if(_pause->Active() == true) _pause->Draw();
		if (_messageServer->IsActive() == true) _messageServer->Draw();
	}

	void ModeMain::ChangeKey(const std::string nextStage) { 
		if (_game.GetModeServer()->IsFadeEnd() == false) return;
		_changeKey = nextStage; 
		_game.GetModeServer()->FadeOut();	// 暗転処理を行う
	}


	bool ModeMain::IsStageChange() {
		// キーは切り替わっているか？
		if (_changeKey == stage::CHANGE_NULL) return false;
		if (CheckSoundMem(se::SoundServer::GetSound(system::MOVE_STAGE1)) == 0) PlaySoundMem(se::SoundServer::GetSound(system::MOVE_STAGE1), DX_PLAYTYPE_BACK);
		if (_game.GetModeServer()->PalChange() == true) {
			// ギミックの状態を更新する
			BgmManage(_changeKey);	// bgm切り替え
			_stageUi->ChangeNumber(_changeKey);
			_fg->SetKey(_changeKey);
			_eServer->Init();	// 各種エフェクトを消去する
			_game.GetScenario()->ScenarioUpdate(_stageKey);	// 元いた情報に更新をかける
			_game.GetMapChips()->ChangeMap(_changeKey);
			_game.GetGimmickServer()->Clear();
			_itemServer->ItemClear();
			_game.GetObjectServer()->ObjectsClear();
			_game.GetObjectServer()->GetPlayer()->SetParameter(_game.GetMapChips()->GetStageTransition()->SetPosition(), _changeKey);	// 自機の座標を更新する
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

	bool ModeMain::BossBattle() {
		if (_stageKey != stage::STAGE_3) return false;
		_bossBattle = true;
		_bg->ScrollOff();

		auto sound = se::SoundServer::GetSound(_bgmKey);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_bgmKey));
	}

	bool ModeMain::BgmManage(std::string nextStage) {
		auto bgm = BgmKey(nextStage);	// キー取得
		if (_bgmKey == bgm)	return false;	// キーが等しい場合はBGMを切り替えず鳴らし続ける
		StopSoundMem(se::SoundServer::GetSound(_bgmKey));	// 現在のキーを止める
		_bgmKey = bgm;	// 違う場合はbgmを切り替える
		if (bgm == bgm::SOUND_STAGE_3) return true;
		PlaySoundMem(se::SoundServer::GetSound(_bgmKey), se::SoundServer::GetPlayType(_bgmKey));
		return true;
	}

	std::string ModeMain::BgmKey(std::string key) {
		if (key == stage::STAGE_0) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_T) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_1) return bgm::SOUND_STAGE_1;
		if (key == stage::STAGE_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_1) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_3) return bgm::SOUND_STAGE_3;
		else return "";	// 登録されていない
	}

	bool ModeMain::GameOver() {
		if (_isReset == true) return false;	// すでに初期化予約が入っている場合は処理を終了する
		_isReset = true;	// フラグを起動
		_game.GetModeServer()->FadeOut();	// 暗転処理を開始
	}

	bool ModeMain::OpenBossStage() {
		if (_bossOpen == true) return false;
		_bossOpen = true;
	}


	bool ModeMain::StageReset() {
		if (_isReset != true) return false;	// フラグが不十分
		if (_game.GetModeServer()->PalChange() != true) return false;	// 時間を満たしていない
		_bossBattle = false;
		if (CheckSoundMem(se::SoundServer::GetSound(bgm::SOUND_STAGE_3)) == 1) StopSoundMem(se::SoundServer::GetSound(bgm::SOUND_STAGE_3));
		// _uiSoul->Init();
		_bg->ScrollOn();	// スクロール再開
		_eServer->Init();	// エフェクトの消去
		_game.GetGimmickServer()->Clear();
		_game.GetObjectServer()->ObjectsClear();	// オブジェクトの消去
		_game.GetObjectServer()->GetPlayer()->Reset();	// 自機をステージの開始地点に戻す
		_game.GetScenario()->AddObjects(_stageKey);		// オブジェクトを再配置する
		_isReset = false;	// 処理終了
	}
}
