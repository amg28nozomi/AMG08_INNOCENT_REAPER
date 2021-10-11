/*****************************************************************//**
 * \file   ImageServer.cpp
 * \brief  イメージサーバクラス
 *		   アイテムテキストを管理する
 * 
 * \author nozom
 * \date   October 2021
 *********************************************************************/
#include "ImageServer.h"
#include "Particle_Image.h"
#include "Game.h"

namespace inr {
	// コンストラクタ
	ImageServer::ImageServer(Game& game) : _game(game) {
		Init();		// 初期化
	}
	// デストラクタ
	ImageServer::~ImageServer() {
		Init();		// 初期化
	}
	// 初期化
	bool ImageServer::Init() {
		// 各種初期化
		ImageClear();
		_imageKey = image::number::NUM;
		_changeKey = image::number::NUM;
		_active = false;
		_input = false;
		return true;
	}
	// コンテナの解放
	bool ImageServer::ImageClear() {
		_images.clear();	// 全要素の解放処理を行う
		return true;
	}
	// 更新
	bool ImageServer::Process() {
		// キーの検索
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false;		// ヒットしない場合は処理処理を終了
		// 描画フラグがない場合
		if (ite->second->IsDraw() != true) {
			_active = false;	// 非活性状態に遷移
			_input = false;		// 入力を受け付けない
			return false;
		}
		// 
		if (_input != true && ite->second->IsNormal() == true) {
			_input = true;
		}
		ite->second->Process();	// 現在のキーの処理を実行する
		Input();
		return true;
	}

	bool ImageServer::Draw() {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false;
		ite->second->Draw();
		return true;
	}

	bool ImageServer::ChangeKey() {
		// キーに変更が加わっている場合のみ、変更処理を行う
		if (_changeKey == image::number::NUM) return false;
		ImageInit();
		_imageKey = _changeKey;
		_changeKey = image::number::NUM;
		_active = true;
		return true;
	}

	bool ImageServer::ImageInit() {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return false; // 失敗しました
		ite->second->Init();	// 初期化処理を呼び出し
		return true;
	}

	bool ImageServer::IsResister(const int number) {
		auto ite = _images.find(_imageKey);
		if (ite == _images.end()) return true;	// 登録処理を行う
		return false;	// すでに登録されているため登録処理を行わない
	}

	bool ImageServer::ImageChange(const int nextKey) {
		if (_active == true) return false;
		if (_changeKey == nextKey) return false;
		_changeKey = nextKey;
		return true;
	}

	bool ImageServer::AddImage(const int number, std::shared_ptr<Particle_Image> image) {
		_images.emplace(number, std::move(image));	// 要素の構成
		return true;
	}

	bool ImageServer::Input() {
		if (_input != true) return false;
		auto ite = _images.find(_imageKey);
		if (ite->second->IsNormal() != true) return false;
		if (_game.GetTrgKey() != PAD_INPUT_4) return false;
		ite->second->End();
		_input = false;
		return true;
	}

	bool ImageServer::IsActive() {
		if (_active == true) return true;					// 既に機能している
		if (_changeKey == image::number::NUM) return false;	// 機能していない
		ImageInit();										// 次の画像の初期化処理呼び出し
		_imageKey = _changeKey;								// キーの更新
		_changeKey = image::number::NUM;					// 切り替え用キーを空にする
		_active = true;										// 活動開始
		// 描画を開始する
		auto ite = _images.find(_imageKey);
		ite->second->DrawStart();
		return true;
	}
}