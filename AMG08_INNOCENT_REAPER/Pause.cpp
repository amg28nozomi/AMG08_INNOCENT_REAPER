/*****************************************************************//**
 * @file   Pause.cpp
 * @brief   �|�[�Y�Ǘ��p�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Pause.h"
#include "Game.h"
#include "SoundServer.h"
#include "Particle_Image.h"
#include "Pause_UI.h"
#include "ModeServer.h"
#include <DxLib.h>

namespace {
  constexpr auto ACTIVE = true;
  constexpr auto INACTIVE = false;

  constexpr auto UI_BG = 0;
  constexpr auto UI_CONTINUE = 1;
  constexpr auto UI_CONTROLS = 2;
  constexpr auto UI_QUIT_TO_TITLE = 3;
  constexpr auto UI_CURSOL = 4;
  constexpr auto UI_OPERATION = 5;

}

namespace inr {

  Pause::Pause(Game& game) : _game(game) {
    for (auto number = 0; number < 4; ++number) _uis.emplace_back(std::make_shared<Particle_Image>(_game.GetGame()));
    _uis.emplace_back(std::make_shared<Pause_UI>(_game.GetGame()));
    _uis.emplace_back(std::make_shared<Particle_Image>(_game.GetGame()));
    // �`����W�C��
    _uis[UI_BG]->SetParameter(image::particle::BG_BLACK, { 960, 540 });                                              // �w�i(UI)
    _uis[UI_CONTINUE]->SetParameter(image::particle::CONTINUE, {960 ,300});                                          // �Q�[���𑱂���(UI)
    _uis[UI_CONTROLS]->SetParameter(image::particle::CONTROLS, {960, 420});                                          // ������@��(UI)
    _uis[UI_QUIT_TO_TITLE]->SetParameter(image::particle::QUIT_TO_TITLE, {960, 540});                                // �^�C�g���ɖ߂�(UI)
    std::dynamic_pointer_cast<Pause_UI>(_uis[UI_CURSOL])->SetParameter(image::particle::CURSOR, {960, 300}, 0, 1.2); // �J�[�\��(UI)
    _uis[UI_OPERATION]->SetParameter(image::particle::BG_OPERATION, { WINDOW_W / 2, WINDOW_H / 2 });                 // ������@(�摜)
    Pause::Init();
  }

  Pause::~Pause() {
  }

  void Pause::Init() {
    _active = INACTIVE;
    _isEnd = false;
    _input = false;
    for (auto number = 0; number < _uis.size(); ++number) _uis[number]->Init();
  }

  void Pause::Process() {
    if (_active == INACTIVE) return; // �񊈐���Ԃ̏ꍇ�͏������s��Ȃ�
    if (_input != true && _uis[UI_QUIT_TO_TITLE]->IsNormal() == true && _uis[UI_OPERATION]->IsDraw() != true) _input = true;
    if (_isEnd == true && _uis[UI_CURSOL]->IsDraw() == false) {
      Init();
      return;
    }
    InputLever();
    InputButton();
    for (auto& ui : _uis) ui->Process();
  }

  void Pause::Draw() {
    for (auto& ui : _uis) ui->Draw();
  }

  bool Pause::IsActive() {
    PauseOn();
    return _active;
  }

  bool Pause::PauseOn() {
    if (_active == true) return false;
    if (_game.GetTrgKey() != PAD_INPUT_13) return false; // ���͖���
    Sound(system::SOUND_PAUSE);
    // �`��J�n
    for (auto i = 0; i < _uis.size() - 1; ++i) if(i != UI_OPERATION) _uis.at(i)->DrawStart();
    _active = true;
    return true; // ������ԂɈڍs
  }

  bool Pause::InputLever() {
    if (_isEnd == true || _input != true) return false; // �I������ or ���͂��󂯕t���Ă��Ȃ��ꍇ�͓��͂��󂯕t���Ȃ�
    // �A�i���O�X�e�B�b�N�̓��͏����擾
    auto lever = _game.GetLeverUD();
    // �����|�����Ȃ� or ����Ȃ��ꍇ�͏������s��Ȃ�
    if (-50 < lever && lever < 50) return false;
    auto type = false;
    if (lever < -50) type = false;
    else type = true;
    // �ԍ���؂�ς���
    std::dynamic_pointer_cast<Pause_UI>(_uis.at(UI_CURSOL))->ChangePosition(type);
    return true;
  }

  bool Pause::InputButton() {
    if (std::dynamic_pointer_cast<Pause_UI>(_uis.at(UI_CURSOL))->IsMove() == true) return false;  // �ړ�����������ꍇ�͏I��
    if (_isEnd == true) return false;      // �I������������ꍇ���������s��Ȃ�
    auto key = _game.GetTrgKey();
    if (key != PAD_INPUT_3 ) return false; // A�{�^����������Ă��Ȃ����͏������I��
    // ���݂̔ԍ��ɉ����āA�������s��
    switch (std::dynamic_pointer_cast<Pause_UI>(_uis.at(UI_CURSOL))->UiNumber()) {
    case system::CONTINUE: // �|�[�Y��ʂ��I�����A�Q�[���{�҂ɖ߂�B
      if (_input != true) return false;    // ���͕��ׂ̏ꍇ�͏������s��Ȃ�
      for (auto i = 0; i < _uis.size(); ++i) _uis.at(i)->End();
      _isEnd = true;
      return true;
    case system::CONTROLS: // ������@�����̕\���؂�ւ�
      if (_uis[UI_OPERATION]->IsDraw() != true && _input == true) {
        // �`����s���Ă��Ȃ��{�͕`����J�n����
        _uis[UI_OPERATION]->DrawStart(); // �`�揈�����I���ɂ���
        _input = false;
        return true;
      }
      else if (_uis[UI_OPERATION]->IsNormal() == true && _input != true) {
        // �`����s���Ă���ꍇ�͕`����I������
        _uis[UI_OPERATION]->End();       // �`�揈�����I������
        _input = true;
        return true;
      }
      return false;
    case system::QUIT_TO_TITLE: // �Q�[���{�ҏI��
      if (_input != true) return false;  // ���͕��ׂ̏ꍇ�͏������s��Ȃ�
      _game.GetModeServer()->ModeChange(mode::TITLE); // �^�C�g���ɑJ�ڂ���
      _isEnd = true;
      // SE��炷
      auto sound = se::SoundServer::GetSound(system::SOUND_SELECT);
      PlaySoundMem(sound, se::SoundServer::GetPlayType(system::SOUND_SELECT));
      return true;
    }
    return false;
  }

  void Pause::Sound(std::string soundKey) {
    auto sound = se::SoundServer::GetSound(soundKey);
    PlaySoundMem(sound, se::SoundServer::GetPlayType(soundKey));
    return;
  }
}