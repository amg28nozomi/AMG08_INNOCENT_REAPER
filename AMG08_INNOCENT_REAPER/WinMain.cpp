/*****************************************************************//**
 * @file   WinMain.cpp
 * @brief  本プログラムのエントリーポイント
 * 
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include <DxLib.h>
#include <memory>
#include "Game.h"
#include "ResourceServer.h"
#include "SoundServer.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
  // ログの出力を行わない
  SetOutApplicationLogValidFlag(FALSE);
#ifndef _DEBUG
  // Release時のウィンドウ設定
  ChangeWindowMode(FALSE);
  SetGraphMode(inr::WINDOW_W, inr::WINDOW_H, inr::WINDOW_B);
#endif
#ifdef _DEBUG
  // Debug時のウィンドウ設定
  ChangeWindowMode(TRUE);
  SetGraphMode(inr::WINDOW_W, inr::WINDOW_H, inr::WINDOW_B);
#endif

  DxLib_Init();                  // DXライブラリの初期化
  SetDrawScreen(DX_SCREEN_BACK); // 描画先グラフィック領域の指定

  auto game = std::make_unique<inr::Game>();
  // 1フレームループ
  while (ProcessMessage() != -1) {
#ifdef _DEBUG
    // Debugビルド時はEscapeキーでループ終了
    if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
#endif
    // 終了フラグが真の場合はループ終了
    if (game->GetEndFlag() == inr::GAME_END) break;

    game->Input();
    game->Process();
    game->Draw();
  }

  graph::ResourceServer::Release(); // 画像素材の解放
  se::SoundServer::Release();       // 音源素材の解放

  DxLib::DxLib_End(); // DXライブラリの終了処理

  return 0;
}