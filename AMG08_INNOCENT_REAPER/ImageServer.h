#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "ImageValue.h"

namespace inr {

	namespace image {
		namespace number {
			constexpr auto NUM = -1;
		}
	}

	class Particle_Image;
	class ImageValue;

	using ImageDatas = std::unordered_map<int, ImageValue>;

	class ImageServer {
	public:
		ImageServer();
		~ImageServer();

		bool Init();	// 初期化
		bool Process();	// 更新
		bool Draw();	// 描画

		bool ImageChange(const int nextKey);
	private:
		int _imageKey;	// 検索用キー
		int _changeKey;	// 切り替え用キー
		std::unordered_map<int, std::shared_ptr<Particle_Image>> _images;

		bool ImageClear();	// 登録情報の解放
		bool ImageInit();	// 画像の初期化
		bool ChangeKey();	// 検索用キーを切り替えるか？
	};
}

