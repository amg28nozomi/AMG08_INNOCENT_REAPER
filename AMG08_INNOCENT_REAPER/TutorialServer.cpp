/*****************************************************************//**
 * @file   TutorialServer.cpp
 * @brief  �`���[�g���A���摜���Ǘ�����T�[�o
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "TutorialServer.h"
#include "TutorialImage.h"

namespace inr {

  TutorialServer::TutorialServer() {
    Clear();
  }

  TutorialServer::~TutorialServer() {
    Clear();
  }

  bool TutorialServer::Clear() {
    _isActive = false;
    _images.clear();
    return true;
  }

  void TutorialServer::Add(std::unique_ptr<TutorialImage> tImage) {
    _isActive = true;
    _images.emplace_back(std::move(tImage));
  }

  void TutorialServer::Process() {
    if (_isActive == false) return;
    for (auto&& image : _images) {
      image->Process();
    }
  }

  void TutorialServer::Draw() {
    if (_isActive == false) return;
    for (auto&& image : _images) {
      image->Draw();
    }
  }
}