/*****************************************************************//**
 * @file   StageTransition.h
 * @brief  �X�e�[�W�J�ڏ��̊Ǘ����s���T�[�o
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include "Vector2.h"

namespace inr {
	/** �X�e�[�W�̃R���p�C�����萔 */
	namespace stage {
		// �X�e�[�W�ǂݍ��ݗp�̃L�[
		constexpr auto CHANGE_NULL = "change_null";	//!< �ʏ��Ԃ̃L�[�؂�ւ�
		constexpr auto STAGE_0 = "stage0";					//!< �ŏ�
		constexpr auto STAGE_T = "stage0-2";				//!< �`���[�g���A��
		constexpr auto STAGE_1 = "stage1";					//!< �X�e�[�W1(�p��)
		constexpr auto STAGE_2 = "stage2";					//!< �X�e�[�W2(�X��-����)
		constexpr auto STAGE_2_1 = "stage2-1";			//!< �X�e�[�W2(�X��-�㕔)
		constexpr auto STAGE_2_2 = "stage2-2";			//!< �X�e�[�W2(�X��-����)
		constexpr auto STAGE_3 = "stageb";					//!< �X�e�[�W3(�{�X��)
	}
	/** ��d�C���N���[�h�h�~ */
	class Game;
	class Transition;
	// �X�e�[�W�J�ڏ��̃T�[�o
	class StageTransition {
	public:
		/**
		 * @brief			�R���X�g���N�^
		 * @param			�Q�[���N���X�̎Q��
		 */
		StageTransition(Game& game);
		/**
		 * @brief			�f�X�g���N�^
		 */
		~StageTransition();
		/**
		 * @brief			����������
		 * @return		true��Ԃ�
		 */
		bool Init();
		/**
		 * @brief			�J�ڃ`�b�v�ƏՓ˂����̂��̔���
		 * @param no	�`�b�v�ԍ�
		 * @return		�ڐG���Ă���ꍇ��true��Ԃ�
		 *						�ڐG���Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsHit(const int no);
		/**
		 * @brief			�J�ڏ������s�����̔���
		 * @return		�J�ڏ������s���ꍇ��true��Ԃ�
		 *						�X�V�������Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsStageChange();
		/**
		 * @brief			�J�ڌ���̎擾
		 * @return		����:�J�ڌ�̍��W�@�E��:����
		 */
		std::pair<Vector2, bool> SetPosition();
	private:
		Game& _game;																				//!< �Q�[��
		std::unordered_map<int, Transition> _transitions;		//!< �X�e�[�W�J�ڏ��p�R���e�i
		int _number;																				//!< �ڐG�����`�b�v
		/**
		 * @brief			�R���e�i�̉��
		 */
		void Clear();
	};
}

