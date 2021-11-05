/*****************************************************************//**
 * @file   ModeBase.cpp
 * @brief  モードのスーパークラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ModeBase.h"
#include "Game.h"
#include "ObjectServer.h"
#include "SoundServer.h"

namespace inr {

  int ModeBase::_modeFrame = 0;

  ModeBase::ModeBase(Game& game) : _game(game), _resetFlag(true){
    _bgmKey = "";
    Init();
  }

  ModeBase::~ModeBase() {
  }

  void ModeBase::Init() {
    // フラグがオンの時だけカウント初期化処理を実行
    if (_resetFlag) {
      TimeClear();
    }
  }

  void ModeBase::Process() {
    ++_modeFrame;
  }

  void ModeBase::Draw() {
  }

  void ModeBase::ChangeKey(const std::string nextStage) {
  }

  bool ModeBase::BgmManage() {
    auto sound = se::SoundServer::GetSound(_bgmKey);
    if (CheckSoundMem(sound) == TRUE) {
      StopSoundMem(sound); // BGM停止
      return true;
    }
    else { // 再生していない場合は再生を開始する
      PlaySoundMem(sound, se::SoundServer::GetPlayType(_bgmKey));
      return false;
    }
  }
}