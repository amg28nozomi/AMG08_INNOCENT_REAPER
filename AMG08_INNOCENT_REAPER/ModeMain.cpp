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
#include "TutorialServer.h"
#include <memory>
#include <unordered_map>

namespace {
	constexpr auto WINDOW_MIN = 0;
	constexpr auto END_STAY = 300;
}

namespace inr {

	ModeMain::ModeMain(Game& game) : ModeBase(game) {
		_resetFlg = true;
		_bossBattle = false;
		_bossOpen = false;
		_isEnding = false;
		_stageKey = stage::CHANGE_NULL;
		_changeKey = stage::CHANGE_NULL;

		_bg = std::make_unique<BackGround>(_game.GetGame());
		_stageUi = std::make_unique<StageUi>(_game.GetGame());
		_pause = std::make_unique<Pause>(_game.GetGame());
		_eServer = std::make_shared<EffectServer>(_game.GetGame());
		_messageServer = std::make_unique<ImageServer>(_game.GetGame());
		_fg = std::make_unique<ForeGround>(_game.GetGame());
		_itemServer = std::make_unique<ItemServer>();
		_tutorialServer = std::make_unique<TutorialServer>();
	}

	ModeMain::~ModeMain() {

	}

	void ModeMain::Init() {
		if (_resetFlg) {
			// 初期化後のステージを設定
#ifdef _DEBUG
			_stageKey = stage::STAGE_2;
#endif
#ifndef _DEBUG
			_stageKey = stage::STAGE_0;
#endif
			// ステージ切り替え用のキーを初期化
			_changeKey = stage::CHANGE_NULL;
			BgmManage(_stageKey);	// ステージに対応するBGMを鳴らす

			TimeClear();
			_bg->ScrollOn();													// 背景のスクロール再開
			_fg->SetKey(_stageKey);											// 前景の切り替え
			_game.GetMapChips()->ChangeMap(_stageKey);	// マップチップの切り替え
			_game.GetScenario()->AddObjects(_stageKey);	// 各種オブジェクトの生成
			_eServer->Init();														// エフェクト初期化
			_stageUi->ChangeNumber(_stageKey);					// ステージUIの画像切り替え
			// 各種フラグの設定
			_bossOpen = false;	
			_bossBattle = false;
			_resetFlg = false;
			_isEnding = false;
			_endCount = 0;
		} else {
			_game.GetScenario()->Init();
			_eServer->Init();	// 各種エフェクトを消去する
			_game.GetObjectServer()->AllClear();
			_game.GetGimmickServer()->Clear();
			_pause->Init();
			_messageServer->Init();
			_itemServer->ItemClear();	// 全消去
			_bg->ChangeGraph();
			_tutorialServer->Clear();
			_resetFlg = true;

			auto nowbgm = se::SoundServer::GetSound(_bgmKey);
			StopSoundMem(nowbgm);
		}
	}

	void ModeMain::Process() {
		IsStageChange();	// ステージを切り替えるか？
		StageReset();			// ステージを初期化するか？
		if (_isEnding == true) IsEnding();	// エンディングに突入するか？
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
		_tutorialServer->Process();
		_game.GetMapChips()->Process();
		_eServer->Process();
		_game.GetObjectServer()->Process();
		_itemServer->Process();
		_fg->Process();
		_stageUi->Process();
	}

	void ModeMain::Draw() {
		_bg->Draw();											// 背景
		_game.GetGimmickServer()->Draw();	// ギミック
		_game.GetMapChips()->Draw();			// マップチップ
		_tutorialServer->Draw();					// チュートリアルUI
		_eServer->DrawBack();							// エフェクト(後)
		_game.GetObjectServer()->Draw();	// オブジェクト
		_itemServer->Draw();							// アイテム
		_eServer->DrawFormer();						// エフェクト(前)
		_fg->Draw();											// 前景
		_stageUi->Draw();									// ステージUI
		// ポーズ画面
		if(_pause->Active() == true) _pause->Draw();
		// アイテムテキスト
		if (_messageServer->IsActive() == true) _messageServer->Draw();
	}

	void ModeMain::ChangeKey(const std::string nextStage) { 
		if (_game.GetModeServer()->IsFadeEnd() == false) return;
		_changeKey = nextStage; 
		_game.GetModeServer()->FadeOut();	// 暗転処理を行う
		_game.GetObjectServer()->GetPlayer()->InputOff();
	}


	bool ModeMain::IsStageChange() {
		// キーは切り替わっているか？
		if (_changeKey == stage::CHANGE_NULL) return false;
		// ステージ遷移SEを鳴らす
		if (CheckSoundMem(se::SoundServer::GetSound(system::MOVE_STAGE1)) == 0) PlaySoundMem(se::SoundServer::GetSound(system::MOVE_STAGE1), DX_PLAYTYPE_BACK);
		// 画面が完全に暗転したら各種初期化を行う
		if (_game.GetModeServer()->PalChange() == true) {
			// ギミックの状態を更新する
			BgmManage(_changeKey);	// bgm切り替え
			_stageUi->ChangeNumber(_changeKey);
			_fg->SetKey(_changeKey);
			_eServer->Init();	// 各種エフェクトを消去する
			_tutorialServer->Clear();
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
	}
	// キーが空かの判定
	bool ModeMain::IsKeyNull() {
		// キーは切り替わっているか
		bool flag = _changeKey == stage::CHANGE_NULL;
		return flag;
	}
	// ボス戦開始フラグ
	bool ModeMain::BossBattle() {
		// ボスステージではない場合は終了
		if (_stageKey != stage::STAGE_3) return false;
		// エンディングがある場合も処理を中断
		if (_isEnding == true) return false;
		_bossBattle = true;								// ボス戦開始
		_bg->ScrollOff();									// スクロールを停止
		_game.GetScenario()->BossBlock();	// ブロックを生成して道を封鎖する

		auto sound = se::SoundServer::GetSound(_bgmKey);
		PlaySoundMem(sound, se::SoundServer::GetPlayType(_bgmKey));
	}
	// ボス戦を終了するか
	bool ModeMain::BossEnd() {
		// 現在居るのはボスステージか？
		if (_stageKey != stage::STAGE_3) return false;
		// ボス戦中か?
		if (_bossBattle != true) return false;
		_bossBattle = false;		// ボス戦を終了する
		_isEnding = true;				// エンドフラグを立てる
		_endCount = END_STAY;		// エンディングに遷移するまでの残りフレーム数
		// ボス専BGMを停止する
		StopSoundMem(se::SoundServer::GetSound(_bgmKey));
		return true;	// ボス戦終了
	}
	// BGMの切り替え
	bool ModeMain::BgmManage(std::string nextStage) {
		auto bgm = BgmKey(nextStage);
		if (_bgmKey == bgm)	return false;									// キーが等しい場合はBGMを切り替えず鳴らし続ける
		StopSoundMem(se::SoundServer::GetSound(_bgmKey));	// 現在のキーを止める
		// 違う場合はbgmを切り替える
		_bgmKey = bgm;
		if (bgm == bgm::SOUND_STAGE_3) return true;
		PlaySoundMem(se::SoundServer::GetSound(_bgmKey), se::SoundServer::GetPlayType(_bgmKey));
		return true;
	}
	// BGMキーの切り替え
	std::string ModeMain::BgmKey(std::string key) {
		// キーと対応するサウンドキーを返す
		if (key == stage::STAGE_0) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_T) return bgm::SOUND_STAGE_0;
		if (key == stage::STAGE_1) return bgm::SOUND_STAGE_1;
		if (key == stage::STAGE_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_1) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_2_2) return bgm::SOUND_STAGE_2;
		if (key == stage::STAGE_3) return bgm::SOUND_STAGE_3;
		else return "";	// 登録されていない
	}
	// ゲームオーバー
	bool ModeMain::GameOver() {
		if (_isReset == true) return false;	// すでに初期化予約が入っている場合は処理を終了する
		_isReset = true;										// フラグを起動
		_game.GetModeServer()->FadeOut();		// 暗転処理を開始
		return true;
	}
	// ボスドアフラグの開放
	bool ModeMain::OpenBossStage() {
		// 既に開いている場合は終了
		if (_bossOpen == true) return false;
		_bossOpen = true;
		return true;		// 開放成功
	}
	// エンディングに遷移するか
	bool ModeMain::IsEnding() {
		if (_endCount == 0) return true;
		--_endCount;
		if (_endCount == 0) {
			// カウンタが0になった場合は120フレーム後に遷移処理開始
			_game.GetModeServer()->ModeChange(mode::FIN, 120);
			return true;
		}
		return false;
	}
	// ステージのリセット
	bool ModeMain::StageReset() {
		if (_isReset != true) return false;														// フラグが不十分
		if (_game.GetModeServer()->PalChange() != true) return false;	// 時間を満たしていない
		_bossBattle = false;														//　ボス戦を終了する
		// ボス戦BGMが鳴っている場合は停止する
		if (CheckSoundMem(se::SoundServer::GetSound(bgm::SOUND_STAGE_3)) == 1) StopSoundMem(se::SoundServer::GetSound(bgm::SOUND_STAGE_3));
		// _uiSoul->Init();
		_bg->ScrollOn();																// スクロール再開
		_eServer->Init();																// エフェクトの消去
		_tutorialServer->Clear();												// チュートリアルUIの消去
		_itemServer->ItemClear();												// アイテム初期化
		_game.GetGimmickServer()->Clear();							// ギミックの消去
		_game.GetObjectServer()->ObjectsClear();				// オブジェクトの消去
		_game.GetObjectServer()->GetPlayer()->Reset();	// 自機をステージの開始地点に戻す
		_game.GetScenario()->AddObjects(_stageKey);			// オブジェクトを再配置する
		_isReset = false;																// 処理終了
		return true;
	}
}
