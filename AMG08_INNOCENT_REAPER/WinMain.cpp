/*****************************************************************//**
 * @file   WinMain.cpp
 * @brief  �{�v���O�����̃G���g���[�|�C���g
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include <DxLib.h>
#include <memory>
#include "Game.h"
#include "ResourceServer.h"
#include "SoundServer.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
  // ���O�̏o�͂��s��Ȃ�
  SetOutApplicationLogValidFlag(FALSE);
#ifndef _DEBUG
  // Release���̃E�B���h�E�ݒ�
  ChangeWindowMode(FALSE);
  SetGraphMode(inr::WINDOW_W, inr::WINDOW_H, inr::WINDOW_B);
#endif
#ifdef _DEBUG
  // Debug���̃E�B���h�E�ݒ�
  ChangeWindowMode(TRUE);
  SetGraphMode(inr::WINDOW_W, inr::WINDOW_H, inr::WINDOW_B);
#endif

  DxLib_Init();                  // DX���C�u�����̏�����
  SetDrawScreen(DX_SCREEN_BACK); // �`���O���t�B�b�N�̈�̎w��

  auto game = std::make_unique<inr::Game>();
  // 1�t���[�����[�v
  while (ProcessMessage() != -1) {
#ifdef _DEBUG
    // Debug�r���h����Escape�L�[�Ń��[�v�I��
    if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
#endif
    // �I���t���O���^�̏ꍇ�̓��[�v�I��
    if (game->GetEndFlag() == inr::GAME_END) break;

    game->Input();
    game->Process();
    game->Draw();
  }

  graph::ResourceServer::Release(); // �摜�f�ނ̉��
  se::SoundServer::Release();       // �����f�ނ̉��

  DxLib::DxLib_End(); // DX���C�u�����̏I������

  return 0;
}