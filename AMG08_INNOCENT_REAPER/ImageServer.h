#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "ImageValue.h"

namespace inr {

	namespace image {
		namespace number {
			constexpr auto NUM = -1;
		}
	}

	class Particle_Image;
	class ImageValue;
	class Game;

	class ImageServer {
	public:
		ImageServer(Game& game);
		~ImageServer();

		bool Init();	// ������
		bool Process();	// �X�V
		bool Draw();	// �`��

		bool ImageChange(const int nextKey);	// �摜�̐؂�ւ�
		bool IsResister(const int number);
		bool AddImage(const int number, std::shared_ptr<Particle_Image> image);	// �V�K�摜�̓o�^

		bool IsLoad() { return _images.empty(); }
		bool IsActive();
	private:
		Game& _game;
		int _imageKey;	// �����p�L�[
		int _changeKey;	// �؂�ւ��p�L�[
		bool _active;	// ���������s���邩�ǂ���
		bool _input;	// ���͏������󂯕t���邩�ۂ�
		std::unordered_map<int, std::shared_ptr<Particle_Image>> _images;
		bool ImageClear();	// �o�^���̉��

		bool ImageInit();	// �摜�̏�����
		bool ChangeKey();	// �����p�L�[��؂�ւ��邩�H
		bool Input();	// ���͏����̊Ǘ�
	};
}

