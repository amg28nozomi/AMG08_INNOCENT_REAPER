/*****************************************************************//**
 * \file   WinMain.cpp
 * \brief  エントリーポイント
 * 
 * \author 鈴木希海
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
		// デバッグ中のみ、Escapeキーでプログラムを終了
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
#endif
		// 終了フラグがオンの場合は処理から抜ける
		if (game->GetEndFlag() == inr::GAME_END) break;

		game->Input();
		game->Process();
		game->Draw();
	}

	graph::ResourceServer::Release();	// 画像素材の解放
	se::SoundServer::Release();	// 音源素材の解放

	DxLib_End();

	return 0;
}