/*****************************************************************//**
 * @file   ParticleValue.h
 * @brief  �p�[�e�B�N���摜�̏��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once

namespace inr {
	/** �p�[�e�B�N���C���[�W�̃R���p�C�����萔 */
	namespace pvalue {
		// �A�C�e���ԍ�
		constexpr auto ITEM_0 = 0;								//!< �A�C�e��1
		constexpr auto ITEM_1 = 1;								//!< �A�C�e��2
		constexpr auto ITEM_2 = 2;								//!< �A�C�e��3
		constexpr auto ITEM_3 = 3;								//!< �A�C�e��4
	}
	namespace item {
		// �e�L�X�g�ԍ�
		constexpr auto MESSAGE_0 = "message_0";		//!< �e�L�X�g1
		constexpr auto MESSAGE_1 = "message_1";		//!< �e�L�X�g2
		constexpr auto MESSAGE_2 = "message_2";		//!< �e�L�X�g3
		constexpr auto MESSAGE_3 = "message_3";		//!< �e�L�X�g4
	}
	/** �p�[�e�B�N���C���[�W�N���X�̃f�[�^ */
	class ParticleValue {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 */
		ParticleValue();
		/**
		 * @brief						�R���X�g���N�^
		 * @param animation	�A�j���[�V�����������s����
		 * @param particle	���ߏ������s����
		 * @param exrate		�g�k��
		 */
		ParticleValue(bool animation, bool particle, double exrate = 1.0);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~ParticleValue() = default;
		// �A�j���[�V�����t���O�̎擾
		/**
		 * @brief		�A�j���[�V�����t���O�̎擾
		 * @return	�A�j���[�V�����t���O��Ԃ�
		 */
		inline bool Animation() { return _animation; }
		/**
		 * @brief		���ߏ����t���O�̎擾
		 * @return	���ߏ����t���O��Ԃ�
		 */
		inline bool Particle() { return _particle; }
		/**
		 * @brief		�g�嗦�̎擾
		 * @return	�g�嗦��Ԃ�
		 */
		inline double ExRate() { return _exrate; }
	private:
		bool _animation;	//!< �A�j���[�V�����̍Đ����s����
		bool _particle;		//!< ���ߏ������s����
		double _exrate;		//!< �g�嗦�i�f�t�H���g����1.0�{�j
	};
}

