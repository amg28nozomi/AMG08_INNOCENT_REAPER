#pragma once
#include <vector>

namespace inr {

	class ObjectValue;

	class Loads {
	public:
		static void ResourceLoad();	// �e��f�ނ̓ǂݍ���
		static std::vector<ObjectValue> LoadScenarioS();	// �X�e�[�WS
		static std::vector<ObjectValue> LoadScenario1();	// �X�e�[�W1
		static std::vector<ObjectValue> LoadScenario2();	// �X�e�[�W2(����)
		static std::vector<ObjectValue> LoadScenario2_1();	// �X�e�[�W2(�㕔)
		static std::vector<ObjectValue> LoadScenario2_2();	// �X�e�[�W2(����)
		static std::vector<ObjectValue> LoadScenarioB();	// �X�e�[�W�{�X
	private:
	};
}

