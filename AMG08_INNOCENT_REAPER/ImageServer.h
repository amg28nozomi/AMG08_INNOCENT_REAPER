/*****************************************************************//**
 * \file   ImageServer.h
 * \brief  イメージサーバクラス
 *		   アイテムテキストを管理する
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "ImageValue.h"

namespace inr {
	namespace image {
		namespace number {
			// 連想配列検索用キー
			constexpr auto NUM = -1;	// ヒットなし
		}
	}
	// 二重インクルード防止
	class Particle_Image;
	class ImageValue;
	class Game;
	// イメージサーバ
	class ImageServer {
	public:
		// コンストラクタ(引数:ゲームクラスの参照)
		ImageServer(Game& game);
		// デストラクタ
		~ImageServer();
		// 初期化
		bool Init();
		// 更新
		bool Process();
		// 描画
		bool Draw();
		// 描画画像の切り替え(引数1:次に描画する画像のキー)
		bool ImageChange(const int nextKey);
		// 対象の画像番号は登録されているか？
		bool IsResister(const int number);
		// 新規画像の生成および連想配列への登録
		bool AddImage(const int number, std::shared_ptr<Particle_Image> image);
		// 画像の読み込み処理を行うか
		bool IsLoad() { return _images.empty(); }
		// 活動状態にあるかの判定
		bool IsActive();
		// 活動状態の取得
		bool Active() { return _active; }
	private:
		Game& _game;
		int _imageKey;														// 検索用キー
		int _changeKey;														// 切り替え用キー
		bool _active;														// 処理を実行するかどうか
		bool _input;														// 入力処理を受け付けるか否か
		std::unordered_map<int, std::shared_ptr<Particle_Image>> _images;	// 画像を管理するための連想配列
		// 画像の解放
		bool ImageClear();
		// 画像の初期化処理呼び出し
		bool ImageInit();
		// 検索用キーの切り替え
		bool ChangeKey();
		// 入力処理
		bool Input();
	};
}

