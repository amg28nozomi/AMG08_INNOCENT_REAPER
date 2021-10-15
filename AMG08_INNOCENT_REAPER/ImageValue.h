/*****************************************************************//**
 * \file   ImageValue.h
 * \brief  �C���[�W�o�����[�N���X
 *         �p�[�e�B�N���o�����[�N���X
 *		   �`���[�g���A���o�����[�N���X
 *		   �摜�N���X���p�������T�u�N���X�̊e��f�[�^
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>
#include <vector>

namespace inr {
	namespace pvalue {
		// �A�C�e���ԍ�
		constexpr auto ITEM_0 = 0;
		constexpr auto ITEM_1 = 1;
		constexpr auto ITEM_2 = 2;
		constexpr auto ITEM_3 = 3;
	}
	namespace item {
		// �e�L�X�g�ԍ�
		constexpr auto MESSAGE_0 = "message_0";
		constexpr auto MESSAGE_1 = "message_1";
		constexpr auto MESSAGE_2 = "message_2";
		constexpr auto MESSAGE_3 = "message_3";
	}
	// �p�[�e�B�N���C���[�W�N���X�̃f�[�^
	class ParticleValue {
	public:
		// �R���X�g���N�^
		ParticleValue();
		// �R���X�g���N�^(����1:�A�j���[�V�����������s�����@����2:���ߏ������s�����@����3:�g�k��)
		ParticleValue(bool animation, bool particle, double exrate = 1.0);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~ParticleValue() = default;
		// �A�j���[�V�����t���O�̎擾
		inline bool Animation() { return _animation; }
		// ���ߏ������s�����̎擾
		inline bool Particle() { return _particle; }
		// �g�嗦�̎擾
		inline double ExRate() { return _exrate; }
	private:
		bool _animation;	// �A�j���[�V�����̍Đ����s����
		bool _particle;		// ���ߏ������s����
		double _exrate;		// �g�嗦�i�f�t�H���g����1.0�{�j
	};
	// �摜�N���X�̃f�[�^
	class ImageValue {
	public:
		// �R���X�g���N�^
		ImageValue();
		// �R���X�g���N�^(����1:�摜�L�[�@����2:�`����W�@����3:�p�[�e�B�N���C���[�W�̏��)
		ImageValue(std::string gkey, Vector2 pos, ParticleValue paricleValue);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~ImageValue() = default;
		// �摜�L�[�̎擾
		inline std::string GraphKey() { return _graph; }
		// ���W�̎擾
		inline Vector2 Position() { return _position; }
		// �p�[�e�B�N���C���[�W���̎擾
		inline ParticleValue ParticleData() { return _particleValue; }
	private:
		std::string _graph;				// �摜�L�[
		Vector2 _position;				// ���W
		ParticleValue _particleValue;	// �p�[�e�B�N���C���[�W�̏��
	};
	// �`���[�g���A���摜�N���X�̃f�[�^
	class TutorialValue {
	public:
		// �R���X�g���N�^
		TutorialValue();
		// �R���X�g���N�^(����1:�摜���@����2:�����@����3:�c��)
		TutorialValue(ImageValue ivalue, int width = 0, int height = 0);
		// �R���X�g���N�^(����1:�摜���@����2:����(min)�@����3:����(max)�@����4:�c��(min)�@����5:�c��(max))
		TutorialValue(ImageValue ivalue, int width1, int width2, int height1, int height2);
		// �摜���̎擾
		inline ImageValue GetImageValue() { return _ivalue; }
		// �����̎擾
		std::vector<int> Width() { return _width; }
		// �c���̎擾
		std::vector<int> Height() { return _height; }
	private:
		ImageValue _ivalue;			// �C���[�W���
		std::vector<int> _width;	// ����
		std::vector<int> _height;	// �c��
	};
}

