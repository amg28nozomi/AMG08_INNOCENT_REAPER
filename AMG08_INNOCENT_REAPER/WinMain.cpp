#include <DxLib.h>
#include <memory>
#include "Game.h"
#include "ResourceServer.h"
#include "SoundServer.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);

#ifndef _DEBUG
	SetGraphMode(inr::WINDOW_W, inr::WINDOW_H, inr::WINDOW_B);
#endif
#ifdef _DEBUG
	// デバッグ専用表示
	// SetGraphMode(inr::DEBUG_WINDOW_W, inr::DEBUG_WINDOW_H, inr::WINDOW_B);
	SetGraphMode(inr::WINDOW_W, inr::WINDOW_H, inr::WINDOW_B);
#endif

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetMouseDispFlag(FALSE);
	
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

	graph::ResourceServer::Release();
	se::SoundServer::Release();

	DxLib_End();

	return 0;
}