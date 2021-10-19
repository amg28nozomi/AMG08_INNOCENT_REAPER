/*****************************************************************//**
 * @file   ImageServer.h
 * @brief  �C���[�W�T�[�o�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "ImageValue.h"

namespace inr {
	/** �摜�̃R���p�C�����萔 */
	namespace image {
		/**   �e�L�X�g�����p�L�[ */
		namespace number {
			constexpr auto NUM = -1;	//!< �e�L�X�g�摜�Ȃ�
		}
	}
	/** ��d�C���N���[�h�h�~ */
	class Particle_Image;
	class ImageValue;
	class Game;
	/** �C���[�W�T�[�o */
	class ImageServer {
	public:
		/**
		 * @brief					�R���X�g���N�^
		 * @param game		�Q�[���N���X�̎Q��
		 */
		ImageServer(Game& game);
		/**
		 * @brief					�f�X�g���N�^
		 */
		~ImageServer();
		/**
		 * @brief					����������
		 * @return				true��Ԃ�
		 */
		bool Init();
		/**
		 * @brief					�X�V����
		 * @return				�X�V���������������ꍇ��true��Ԃ�
		 *								���f�����ꍇ��false��Ԃ�
		 */
		bool Process();
		/**
		 * @brief					�`�揈��
		 * @return				�`�揈�������������ꍇ��true��Ԃ�
		 *								�R���e�i����̏ꍇ��false��Ԃ�
		 */
		bool Draw();
		/**
		 * @brief					�`��摜�̐؂�ւ�
		 * @param nextKey	���ɕ`�悷��摜�̓o�^�L�[
		 * @return				�؂�ւ��ɐ��������ꍇ��true��Ԃ�
		 *								���s�����ꍇ��false��Ԃ�
		 */
		bool ImageChange(const int nextKey);
		/**
		 * @brief					�o�^�������s�����̔���
		 * @param number	�o�^�ԍ�
		 * @return				�o�^�������s���ꍇ��true��Ԃ�
		 *								���ɓo�^����Ă���ꍇ��false��Ԃ�
		 */
		bool IsResister(const int number);
		/**
		 * @brief					�摜�̓o�^
		 * @param number	�o�^�ԍ�
		 * @param image		�摜�̃V�F�A�[�h�|�C���^
		 * @return				true��Ԃ�
		 */
		bool AddImage(const int number, std::shared_ptr<Particle_Image> image);
		/**
		 * @brief					�摜�̓ǂݍ��ݏ������s����
		 * @return				�R���e�i����̏ꍇ��true��Ԃ�
		 *								�v�f������ꍇ��false��Ԃ�
		 */
		bool IsLoad() { return _images.empty(); }
		/**
		 * @brief					������Ԃ̔���E�ڍs����
		 * @return				������ԂɈڍs�����ꍇ��true��Ԃ�
		 *								���s�����ꍇ��false��Ԃ�
		 */
		bool IsActive();
		/**
		 * @brief					������Ԃ̎擾
		 * @return				�����t���O��Ԃ�
		 */
		bool Active() { return _active; }
	private:
		Game& _game;																											//!< �Q�[���N���X�̎Q��
		int _imageKey;																										//!< �����p�L�[
		int _changeKey;																										//!< �؂�ւ��p�L�[
		bool _active;																											//!< ���������s���邩�ǂ���
		bool _input;																											//!< ���͏������󂯕t���邩�ۂ�
		std::unordered_map<int, std::shared_ptr<Particle_Image>> _images;	//!< �摜���Ǘ����邽�߂̘A�z�z��
		/**
		 * @brief					�摜�̊J��
		 * @return				true��Ԃ�
		 */
		bool ImageClear();
		/**
		 * @brief					�摜�̏����������Ăяo��
		 * @return				�������ɐ��������ꍇ��true��Ԃ�
		 *								���s�����ꍇ��false��Ԃ�
		 */
		bool ImageInit();
		/**
		 * @brief					�����p�L�[�̐؂�ւ�
		 * @return				�؂�ւ��ɐ��������ꍇ��true��Ԃ�
		 *								���s�����ꍇ��false��Ԃ�
		 */
		bool ChangeKey();
		/**
		 * @brief					���͏���
		 * @return				�����ɐ��������ꍇ��true��Ԃ�
		 *								���s�����ꍇ��false��Ԃ�
		 */
		bool Input();
	};
}

