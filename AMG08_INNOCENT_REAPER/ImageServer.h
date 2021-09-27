#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "ImageValue.h"

namespace inr {

	namespace image {
		namespace number {
			constexpr auto NUM = -1;
		}
	}

	class Particle_Image;
	class ImageValue;
	class Game;

	class ImageServer {
	public:
		ImageServer(Game& game);
		~ImageServer();

		bool Init();	// 初期化
		bool Process();	// 更新
		bool Draw();	// 描画

		bool ImageChange(const int nextKey);	// 画像の切り替え
		bool IsResister(const int number);
		bool AddImage(const int number, std::shared_ptr<Particle_Image> image);	// 新規画像の登録

		bool IsLoad() { return _images.empty(); }
		bool IsActive();
	private:
		Game& _game;
		int _imageKey;	// 検索用キー
		int _changeKey;	// 切り替え用キー
		bool _active;	// 処理を実行するかどうか
		bool _input;	// 入力処理を受け付けるか否か
		std::unordered_map<int, std::shared_ptr<Particle_Image>> _images;
		bool ImageClear();	// 登録情報の解放

		bool ImageInit();	// 画像の初期化
		bool ChangeKey();	// 検索用キーを切り替えるか？
		bool Input();	// 入力処理の管理
	};
}

