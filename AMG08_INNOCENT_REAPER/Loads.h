#pragma once
#include <vector>

namespace inr {

	class ObjectValue;

	class Loads {
	public:
		static void ResourceLoad();	// 各種素材の読み込み
		static std::vector<ObjectValue> LoadScenario();	// シナリオの読み込み処理
	private:
	};
}

