/*****************************************************************//**
 * @file   ModeBase.cpp
 * @brief  ���[�h�̃X�[�p�[�N���X
 *
 * @author ��؊�C
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
    // �t���O���I���̎������J�E���g���������������s
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
      StopSoundMem(sound); // BGM��~
      return true;
    }
    else { // �Đ����Ă��Ȃ��ꍇ�͍Đ����J�n����
      PlaySoundMem(sound, se::SoundServer::GetPlayType(_bgmKey));
      return false;
    }
  }
}