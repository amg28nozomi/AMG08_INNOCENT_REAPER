#pragma once
#include <vector>
#include "Scenario.h"
#include "ImageValue.h"

namespace inr {

	class ObjectValue;

	class Loads {
	public:
		static void ResourceLoad();	// �e��f�ނ̓ǂݍ���
		static ImageMap LoadImages();	// �摜�̓ǂݍ���
		static std::vector<ImageValue> LoadForeGround1();	// �O�i

		static std::vector<ObjectValue> LoadScenarioS_1();	// �`���[�g���A��
		static std::vector<ObjectValue> LoadScenarioS();	// �X�e�[�WS
		static std::vector<ObjectValue> LoadScenario1();	// �X�e�[�W1
		static std::vector<ObjectValue> LoadScenario2();	// �X�e�[�W2(����)
		static std::vector<ObjectValue> LoadScenario2_1();	// �X�e�[�W2(�㕔)
		static std::vector<ObjectValue> LoadScenario2_2();	// �X�e�[�W2(����)
		static std::vector<ObjectValue> LoadScenarioB();	// �X�e�[�W�{�X
	private:
	};
}

