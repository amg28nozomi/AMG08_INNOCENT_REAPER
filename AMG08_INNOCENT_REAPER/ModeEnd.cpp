/*****************************************************************//**
 * @file   ModeEnd.cpp
 * @brief  エンディングを管理するモードエンドクラス（モードベースのサブクラス）
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ModeEnd.h"
#include "Game.h"
#include "ModeServer.h"
#include "SoundServer.h"

namespace {
  constexpr auto INTERVAL = 60 * 2;
}

namespace inr {

  ModeEnd::ModeEnd(Game& game) : ModeBase(game), _end(false) {
    _resetFlag = true;
    _staffRoll = std::make_unique<MoveImage>(_game.GetGame());

    auto sh = (end::STAFF_ROLL_HEIGHT / 2);
    _staffRoll->SetParameter(end::STAFF_ROLL, { HALF_WINDOW_W, static_cast<double>(sh) });
    _bgmKey = bgm::SOUND_ENDING; // キーをエンディングに切り替える
    _count = 0;
  }

  void ModeEnd::Init() {
    switch (_resetFlag) {
    case true:
      _end = false;
      _resetFlag = false;
      _count = INTERVAL;
      BgmManage(); // BGMの切り替え
      _staffRoll->Init();
      return;
    case false:
      _resetFlag = true;
      _count = INTERVAL;
      auto sound = se::SoundServer::GetSound(_bgmKey);
      StopSoundMem(sound);
      return;
    }
  }

  void ModeEnd::Process() {
    if (IsEnd() == true) return;
    if (_staffRoll->IsActive() == true) { 
      _staffRoll->Process();
      return;
    }

    if (_count == 0) {
      _end = true;
      return;
    }
    --_count;
  }

  void ModeEnd::Draw() {
    _staffRoll->Draw();
  }

  bool ModeEnd::IsEnd() {
    if (_end == false) return false;
    _game.GetModeServer()->ModeChange(mode::TITLE); // タイトルに遷移する
    return true;
  }
}