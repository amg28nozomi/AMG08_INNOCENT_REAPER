#include "TutorialValue.h"

namespace inr {
	// コンストラクタ
	TutorialValue::TutorialValue() : _ivalue() {
		// コンテナの初期化
		_width.clear();
		_height.clear();
	}
	// コンストラクタ
	TutorialValue::TutorialValue(ImageValue ivalue, int width, int height) : _ivalue(ivalue) {
		// 初期化
		_width.emplace_back(width);
		_height.emplace_back(height);
	}
	// コンストラクタ
	TutorialValue::TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2) : _ivalue(ivalue) {
		// 初期化
		_width = { width1, width2 };
		_height = { height1, height2 };
	}
}
