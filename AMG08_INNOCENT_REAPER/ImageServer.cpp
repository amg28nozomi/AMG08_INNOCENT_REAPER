/*****************************************************************//**
 * @file   ImageServer.cpp
 * @brief  イメージサーバクラス
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "ImageServer.h"
#include "Particle_Image.h"
#include "Game.h"

namespace inr {

	ImageServer::ImageServer(Game& game) : _game(game) {
		Init();
	}

	ImageServer::~ImageServer() {
		Init();
	}

	bool ImageServer::Init() {
		// 各種初期化
		ImageClear();
		_imageKey = image::number::NUM;
		_changeKey = image::number::NUM;
		_active = false;
		_input = false;
		return true;
	}

	bool ImageServer::ImageClear() {
		_images.clear();	// 全要素の解放処理を行う
		return true;
	}

	bool ImageServer::Process() {
		auto ite = _images.find(_imageKey);				// キーの検索
		if (ite == _images.end()) return false;		// ヒットしない場合は処理を終了
		// 描画フラグがない場合
		if (ite->second->IsDraw() != true) {
			_active = false;			// 非活性状態に遷移
			_input = false;				// 入力を受け付けない
			return false;					// 更新終了
		}
		// 画像のアニメーションが行われていない状態かつ、入力状態がオフの場合
		if (_input != true && ite->second->IsNormal() == true) {
			_input = true;				// 入力状態オン
		}
		ite->second->Process();	// 現在のキーの処理を実行する
		Input();								// 入力処理呼び出し
		return true;						// 更新完了
	}

	bool ImageServer::Draw() {
		auto ite = _images.find(_imageKey);				// 画像の取り出し
		if (ite == _images.end()) return false;		// 取り出しに失敗した場合は処理中断
		ite->second->Draw();											// 描画処理呼び出し
		return true;															// 描画成功
	}

	bool ImageServer::ChangeKey() {
		// キーに変更が加わっている場合のみ、変更処理を行う
		if (_changeKey == image::number::NUM) return false;
		ImageInit();											// 画像の初期化
		_imageKey = _changeKey;						// キーの切り替え
		_changeKey = image::number::NUM;	// 切り替え用キーを空にする
		_active = true;										// 活動状態に切り替え
		return true;											// 切り替え成功
	}

	bool ImageServer::ImageInit() {
		auto ite = _images.find(_imageKey);				// 画像の取り出し
		if (ite == _images.end()) return false;		// 取り出し失敗
		ite->second->Init();											// 初期化処理呼び出し
		return true;															// 初期化成功
	}

	bool ImageServer::IsResister(const int number) {
		auto ite = _images.find(_imageKey);				// 画像の取り出し
		if (ite == _images.end()) return true;		// 登録処理を行う
		return false;															// すでに登録されているため登録処理を行わない
	}

	bool ImageServer::ImageChange(const int nextKey) {
		if (_active == true) return false;				// 活動状態の場合は中断
		if (_changeKey == nextKey) return false;	// キーが同じ場合は中断
		_changeKey = nextKey;											// キーの更新
		return true;															// 切り替え成功
	}

	bool ImageServer::AddImage(const int number, std::shared_ptr<Particle_Image> image) {
		_images.emplace(number, std::move(image));	// コンテナに登録
		return true;	// 処理終了
	}

	bool ImageServer::Input() {
		if (_input != true) return false;											// 入力処理を受け付けていない
		auto ite = _images.find(_imageKey);										// 画像の取り出し
		if (ite->second->IsNormal() != true) return false;		// 通常状態ではない場合は処理を行わない
		if (_game.GetTrgKey() != PAD_INPUT_4) return false;		// Bボタンは押されていない
		ite->second->End();																		// 押された場合は終了処理呼び出し
		_input = false;																				// 入力状態をオフにする
		return true;																					// 処理完了
	}

	bool ImageServer::IsActive() {
		if (_active == true) return true;											// 既に機能している
		if (_changeKey == image::number::NUM) return false;		// 機能していない
		ImageInit();																					// 次の画像の初期化処理呼び出し
		_imageKey = _changeKey;																// キーの更新
		_changeKey = image::number::NUM;											// 切り替え用キーを空にする
		_active = true;																				// 活動開始
		// 描画を開始する
		auto ite = _images.find(_imageKey);
		ite->second->DrawStart();
		return true;
	}
}