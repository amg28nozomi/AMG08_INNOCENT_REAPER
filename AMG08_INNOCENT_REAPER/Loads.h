#pragma once
#include <vector>

namespace inr {

	class ObjectValue;

	class Loads {
	public:
		static void ResourceLoad();	// 各種素材の読み込み
		static std::vector<ObjectValue> LoadScenarioS();	// ステージS
		static std::vector<ObjectValue> LoadScenario1();	// ステージ1
		static std::vector<ObjectValue> LoadScenario2();	// ステージ2(中央)
		static std::vector<ObjectValue> LoadScenario2_1();	// ステージ2(上部)
		static std::vector<ObjectValue> LoadScenario2_2();	// ステージ2(下部)
		static std::vector<ObjectValue> LoadScenarioB();	// ステージボス
	private:
	};
}

