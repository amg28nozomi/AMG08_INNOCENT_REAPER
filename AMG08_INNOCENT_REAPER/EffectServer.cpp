/*****************************************************************//**
 * @file   EffectServer.cpp
 * @brief  �G�t�F�N�g�T�[�o�N���X
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "EffectServer.h"
#include "EffectBase.h"

namespace inr {

  EffectServer::EffectServer(Game& game) : _game(game) {
    Clears();
  }

  EffectServer::~EffectServer() {
    Clears();
  }

  void EffectServer::Clears() {
    // �S�R���e�i�̉��
    _effectsF.clear();
    _effectsB.clear();
    _addFormer.clear();
    _addBack.clear();
  }

  void EffectServer::Init() {
    // �e�평�������s
    _update = false;
    _isAdd = false;
    _del = false;
    Clears();
  }

  void EffectServer::Process() {
    // �ꎞ�I�i�[�p�R���e�i�ɃG�t�F�N�g���o�^����Ă���ꍇ�A�i�[�p�R���e�i�ւ̓o�^�����ֈڍs
    if (_isAdd == true) MoveAddEffects();
    IsDelete();       // ���������̊m�F
    _update = true;   // �X�V�J�n
    for (auto&& eff : _effectsB) eff->Process(); // ��i�G�t�F�N�g�̍X�V�����Ăяo��
    for (auto&& eff : _effectsF) eff->Process(); // �O�i�G�t�F�N�g�̍X�V�����Ăяo��
    _update = false;  // �X�V�I��
  }

  void EffectServer::DrawBack() {
    for (auto&& eff : _effectsB) eff->Draw(); // ��i�G�t�F�N�g�̕`�揈���Ăяo��
  }

  void EffectServer::DrawFormer() {
    for (auto&& eff : _effectsF) eff->Draw(); // �O�i�G�t�F�N�g�̕`�揈���Ăяo��
  }

  void EffectServer::Add(std::unique_ptr<EffectBase> eff, int type) {
    // �R���e�i���񂵂Ă��邩�ɉ����Ċi�[�p�R���e�i�ƈꎞ�I�i�[�R���e�i�̂ǂ���ɓo�^���邩����
    // true:�������s���Ă���ꍇ�͓o�^�t���O��^�ɂ��A�Y������ꎞ�I�i�[�R���e�i�ɓo�^
    // false:�������s���Ă��Ȃ��ꍇ�͑Ή�����R���e�i�ɒ��œo�^����
    switch (_update) {
    case true:
      _isAdd = true;
      switch (type) {
      case effect::type::FORMER:
        _addFormer.emplace_back(std::move(eff));
        return;
      case effect::type::BACK:
        _addBack.emplace_back(std::move(eff));
        return;
      default:
        return;
      }
      return;
    case false:
      switch (type) {
      case effect::type::FORMER:
        _effectsF.emplace_back(std::move(eff));
        return;
      case effect::type::BACK:
        _effectsB.emplace_back(std::move(eff));
        return;
      default:
        return;
      }
    }
  }

  void EffectServer::MoveAddEffects() {  
    // �v�f������ꍇ�͑Ή�����R���e�i�ɏ��L�����ڍs����
    if (_addFormer.empty() != true) {
      for(auto&& eff : _addFormer) _effectsF.emplace_back(std::move(eff));
      _addFormer.clear();
    }
    if (_addBack.empty() != true) {
      for(auto&& eff : _addBack)  _effectsB.emplace_back(std::move(eff));
      _addBack.clear();
    }
    _isAdd = false; // �o�^����
  }

  void EffectServer::IsDelete() {
    if (_del != true) return; // �����𖞂����Ă��Ȃ����ߏ����𒆒f
    std::vector<std::unique_ptr<EffectBase>> alives; // ���p�R���e�i
    // �����t���O�������Ă��Ȃ��G�t�F�N�g���ꎞ�I�ɔ�����
    for (auto&& eff : _effectsF) {
      if (eff->IsDel() == true) continue;
      alives.emplace_back(std::move(eff));
    }
    _effectsF.clear();
    // �ēo�^
    for (auto&& eff : alives) _effectsF.emplace_back(std::move(eff));
    alives.clear(); // �R���e�i���
    // �����t���O�������Ă��Ȃ��G�t�F�N�g���ꎞ�I�ɔ�����
    for (auto&& eff : _effectsB) {
      if (eff->IsDel() == true) continue;
      alives.emplace_back(std::move(eff));
    }
    _effectsB.clear();
    // �ēo�^
    for (auto&& eff : alives) _effectsB.emplace_back(std::move(eff));
    _del = false; // ��������
  }
}