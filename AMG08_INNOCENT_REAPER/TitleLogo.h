/*****************************************************************//**
 * @file   TitleLogo.h
 * @brief  �^�C�g�����S�i�摜�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include "Collision.h"
#include "RedGreenBlue.h"
#include <vector>

namespace inr {
	/** �^�C�g�����S */
	class TitleLogo : public Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		TitleLogo(Game& game);
		/**
		 * @brief				����������
		 */
		void Init() override;
		/**
		 * @brief				�X�V����
		 */
		void Process() override;
		/**
		 * @brief				�`�揈��
		 */
		void Draw() override;
	private:
		Vector2 _position2;						//!< ���W
		Collision _hitCol1;						//!< �����蔻��{�b�N�X(1)
		Collision _hitCol2;						//!< �����蔻��{�b�N�X(2)
		int _pal;											//!< �P�x
		std::vector<double> _rate;		//!< �g�嗦
		std::vector<int> _rateFlag;		//!< �g��t���O
		std::string _graphKey2;				//!< �摜�L�[
		/**
		 * @brief				�g�嗦�̍X�V
		 * @return			true��Ԃ�
		 */
		bool RateUpdate();
#ifdef _DEBUG
		/**
		 * @brief				�����蔻��{�b�N�X�̕`��
		 * @param col		�`��Ώۃ{�b�N�X
		 */
		void DebugBox(Collision col);
#endif
		/**
		 * @brief				�Փ˔���t���O�̒�~
		 */
		void CollisionOut();
	};
}

