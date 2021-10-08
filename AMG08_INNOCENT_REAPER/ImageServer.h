/*****************************************************************//**
 * \file   ImageServer.h
 * \brief  �C���[�W�T�[�o�N���X
 *		   �摜�̊Ǘ����s��
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "ImageValue.h"

namespace inr {
	namespace image {
		namespace number {
			// �A�z�z�񌟍��p�L�[
			constexpr auto NUM = -1;	// �q�b�g�Ȃ�
		}
	}
	// ��d�C���N���[�h�h�~
	class Particle_Image;
	class ImageValue;
	class Game;
	// �C���[�W�T�[�o
	class ImageServer {
	public:
		ImageServer(Game& game);
		~ImageServer();
		// ������
		bool Init();
		// �X�V
		bool Process();
		// �`��
		bool Draw();
		// �`��摜�̐؂�ւ�(����1:���ɕ`�悷��摜�̃L�[)
		bool ImageChange(const int nextKey);
		// �Ώۂ̉摜�ԍ��͓o�^����Ă��邩�H
		bool IsResister(const int number);
		// �V�K�摜�̐�������јA�z�z��ւ̓o�^
		bool AddImage(const int number, std::shared_ptr<Particle_Image> image);
		// �摜�̓ǂݍ��ݏ������s����
		bool IsLoad() { return _images.empty(); }
		// 
		bool IsActive();
		bool Active() { return _active; }
	private:
		Game& _game;
		std::unordered_map<int, std::shared_ptr<Particle_Image>> _images;	// �摜���Ǘ����邽�߂̘A�z�z��
		int _imageKey;														// �����p�L�[
		int _changeKey;														// �؂�ւ��p�L�[
		bool _active;														// ���������s���邩�ǂ���
		bool _input;														// ���͏������󂯕t���邩�ۂ�
		// �摜�̉��
		bool ImageClear();
		// �摜�̏����������Ăяo��
		bool ImageInit();
		// �����p�L�[�̐؂�ւ�
		bool ChangeKey();
		// ���͏���
		bool Input();
	};
}

