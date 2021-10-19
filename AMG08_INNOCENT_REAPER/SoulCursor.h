/*****************************************************************//**
 * @file   SoulCursor.h
 * @brief  �^�C�g�����̃J�[�\��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "RedGreenBlue.h"

namespace inr {
	/** UI�̃R���p�C�����萔 */
	namespace ui {
		constexpr auto CURSOR = "ui_cursor";	//!< �J�[�\���̃L�[
	}
	/** �J�[�\�� */
	class SoulCursor : public ObjectBase {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		SoulCursor(Game& game);
		/**
		 * @brief				�f�X�g���N�^
		 */
		~SoulCursor() = default;
		/**
		 * @brief					����������
		 */
		void Init() override;
		/**
		 * @brief					�X�V����
		 */
		void Process() override;
		/**
		 * @brief					�`�揈��
		 */
		void Draw() override;
	private:
		Vector2 _moveVector;	//!< �ړ���
		RedGreenBlue _rgb;		//!< RGB
		bool _input;					//!< ���͉͂\���H	
		bool _setBlend;				//!< �F�ʂ͒����������H
		/**
		 * @brief					�ړ�����
		 * @param lever1	�A�i���O�X�e�B�b�N�̉����͏��
		 * @param lever2	�A�i���O�X�e�B�b�N�̏c���͏��
		 */
		void Move(int lever1, int lever2);
		/**
		 * @brief					���W�̍X�V
		 */
		void PositionUpdate();
		/**
		 * @brief					RGB�l�̐؂�ւ�
		 */
		void ChangeBlendGraph();
	};
}

