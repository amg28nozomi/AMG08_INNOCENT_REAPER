#pragma once
#include "Image.h"
#include "Vector2.h"
#include <vector>

namespace inr {

	namespace stage {
		namespace number {
			constexpr auto SN_NULL = -1;
			constexpr auto SN_S = 0;
			constexpr auto SN_1 = 1;
			constexpr auto SN_2 = 2;
			constexpr auto SN_B = 3;
		}
	}

	class BackGround : public Image{
	public:
		BackGround(Game& game);
		
		void Init() override;
		void Process() override;
		void Draw() override;

		void ChangeGraph();	// �X�e�[�W�ɉ������摜�ɐ؂�ւ���i����:���݂̃X�e�[�W�j
		bool IsChanege();	// �L�[�͐؂�ւ�������H
		int KeyNumber();
	private:
		std::pair<std::vector<Vector2>, std::vector<Vector2>> _positions;	// �`����W(����:1���ڂ̕`����W, �E��2���ڂ̕`����W)
		std::vector<double> _scrSpeed;	// �X�N���[���ړ���
		int _stageNo;	// ���݂̃X�e�[�W

		std::string zKey;	// �O�i
		void BackDraw();	// �O�i�̕`��
	};
}

