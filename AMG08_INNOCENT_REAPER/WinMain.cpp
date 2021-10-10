/*****************************************************************//**
 * \file   WinMain.cpp
 * \brief  �G���g���[�|�C���g
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#include <DxLib.h>
#include <memory>
#include "Game.h"
#include "ResourceServer.h"
#include "SoundServer.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	SetOutApplicationLogValidFlag(FALSE);
#ifndef _DEBUG
	ChangeWindowMode(FALSE);
	SetGraphMode(inr::WINDOW_W, inr::WINDOW_H, inr::WINDOW_B);
#endif
#ifdef _DEBUG
	ChangeWindowMode(TRUE);
	SetGraphMode(inr::WINDOW_W, inr::WINDOW_H, inr::WINDOW_B);
#endif

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	
	auto game = std::make_unique<inr::Game>();

	while (ProcessMessage() != -1) {
#ifdef _DEBUG
		// �f�o�b�O���̂݁AEscape�L�[�Ńv���O�������I��
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
#endif
		// �I���t���O���I���̏ꍇ�͏������甲����
		if (game->GetEndFlag() == inr::GAME_END) break;

		game->Input();
		game->Process();
		game->Draw();
	}

	graph::ResourceServer::Release();	// �摜�f�ނ̉��
	se::SoundServer::Release();	// �����f�ނ̉��

	DxLib_End();

	return 0;
}