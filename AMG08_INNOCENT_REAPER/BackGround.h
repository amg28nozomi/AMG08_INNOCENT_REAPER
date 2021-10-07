#pragma once
#include "Image.h"
#include "Vector2.h"
#include <vector>

namespace inr {

	namespace stage {
		// �X�e�[�W�ԍ�
		namespace number {
			constexpr auto SN_NULL = -1;	// �Y���Ȃ�
			constexpr auto SN_S = 0;		// �X�e�[�W0
			constexpr auto SN_T = 1;		// �`���[�g���A���X�e�[�W
			constexpr auto SN_1 = 2;		// �X�e�[�W1
			constexpr auto SN_2 = 3;		// �X�e�[�W2
			constexpr auto SN_B = 4;		// �{�X�X�e�[�W
		}
	}

	// �w�i
	class BackGround : public Image{
	public:
		BackGround(Game& game);
		
		// ������
		void Init() override;
		// �X�V
		void Process() override;
		//�`��
		void Draw() override;

		// �X�e�[�W�ɉ������摜�ɐ؂�ւ���i����:���݂̃X�e�[�W�j
		void ChangeGraph();
		// �L�[�͐؂�ւ�������H
		bool IsChanege();
		// �X�e�[�W�ԍ��̔���
		int KeyNumber();

		// �X�N���[���I��
		inline void ScrollOff() { _scroll = false; }
		// �X�N���[���ĊJ
		inline void ScrollOn() { _scroll = true; }
	private:
		std::pair<std::vector<Vector2>, std::vector<Vector2>> _positions;	// �`����W(����:1���ڂ̕`����W, �E��2���ڂ̕`����W)
		std::pair<int, int> _fix;	// �`��C���l
		std::pair<std::vector<double>, std::vector<double>> _scrSpeed;	// �X�N���[���ړ���(����:x, �E��:y)
		int _stageNo;		// ���݂̃X�e�[�W
		bool _scroll;		// �X�N���[�����邩

		std::string zKey;	// �O�i
		// �O�`��
		void BackDraw();
		// �X�N���[������(��)
		void BigManage();
		// �X�N���[������(��)
		void NormalManage();
		// Y���W�̏C��
		void ScrollY();
	};
}

