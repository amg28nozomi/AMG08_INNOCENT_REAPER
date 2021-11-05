/*****************************************************************//**
 * @file   TutorialServer.cpp
 * @brief  チュートリアル画像を管理するサーバ
 *
 * @author 鈴木希海
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