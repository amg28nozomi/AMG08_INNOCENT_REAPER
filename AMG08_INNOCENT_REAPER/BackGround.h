/*****************************************************************//**
 * \file   BackGround.h
 * \brief  �w�i�N���X�i�摜�N���X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "Vector2.h"
#include <vector>

namespace inr {
	/** �X�e�[�W�̃R���p�C�����萔 */
	namespace stage {
		/** �X�e�[�W�ԍ��̃R���p�C�����萔 */
		namespace number {
			constexpr auto SN_NULL = -1;	//!< �Y���Ȃ�
			constexpr auto SN_S = 0;			//!< �X�e�[�W0
			constexpr auto SN_T = 1;			//!< �`���[�g���A���X�e�[�W
			constexpr auto SN_1 = 2;			//!< �X�e�[�W1
			constexpr auto SN_2 = 3;			//!< �X�e�[�W2
			constexpr auto SN_B = 4;			//!< �{�X�X�e�[�W
		}
	}
	/** �w�i */
	class BackGround : public Image{
	public:
		/**
		 * @brief	�R���X�g���N�^
		 * @param	game	�Q�[���N���X�̎Q��
		 */
		BackGround(Game& game);		
		/**
		 * @brief	����������
		 */
		void Init() override;
		/**
		 * @brief	�X�V����
		 */
		void Process() override;
		/**
		 * @brief	�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief	�X�e�[�W�ɉ������w�i�ɐ؂�ւ���
		 */
		void ChangeGraph();
		/**
		 * @brief		�L�[�͐؂�ւ�������H
		 * @return	�摜�ԍ����قȂ�ꍇ��true��Ԃ�
		 * @return	�摜�ԍ����������ꍇ��false��Ԃ�
		 */
		bool IsChanege();
		/**
		 * @brief		�X�e�[�W�ԍ��̔��肨��ю擾
		 * @return	�q�b�g�����ꍇ�͑Ή�����X�e�[�W�ԍ���Ԃ�
		 * @return	�q�b�g���Ȃ������ꍇ��-1��Ԃ�
		 */
		int KeyNumber();
		/**
		 * @brief	�X�N���[�������I��
		 */
		inline void ScrollOff() { _scroll = false; }
		/**
		 * @brief	�X�N���[�������ĊJ
		 */
		inline void ScrollOn() { _scroll = true; }
	private:
		std::pair<std::vector<Vector2>, std::vector<Vector2>> _positions;	//!< �`����W(����:1���ڂ̕`����W, �E��2���ڂ̕`����W)
		std::pair<std::vector<double>, std::vector<double>> _scrSpeed;		//!< �X�N���[���ړ���(����:x, �E��:y)
		std::pair<int, int> _fix;																					//!< �`��C���l
		int _stageNo;																											//!< ���݂̃X�e�[�W
		bool _scroll;																											//!< �X�N���[�����邩
		std::string zKey;																									//!< �O�i
		/**
		 * @brief	�O�`�揈��
		 */
		void BackDraw();
		/**
		 * @brief	�X�N���[������(��)
		 */
		void BigManage();
		/**
		 * @brief	�X�N���[������(��)
		 */
		void NormalManage();
		/**
		 * @brief	Y���W�̏C��.
		 */
		void ScrollY();
	};
}

