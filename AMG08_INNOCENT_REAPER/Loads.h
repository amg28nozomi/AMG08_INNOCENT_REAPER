#pragma once
#include <vector>

namespace inr {

	class ObjectValue;

	class Loads {
	public:
		static void ResourceLoad();	// �e��f�ނ̓ǂݍ���
		static std::vector<ObjectValue> LoadScenario();	// �V�i���I�̓ǂݍ��ݏ���
	private:
	};
}

