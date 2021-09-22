#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "ImageValue.h"

namespace inr {

	namespace image {
		namespace number {
			constexpr auto NUM = -1;
		}
	}

	class Particle_Image;
	class ImageValue;

	using ImageDatas = std::unordered_map<int, ImageValue>;

	class ImageServer {
	public:
		ImageServer();
		~ImageServer();

		bool Init();	// ������
		bool Process();	// �X�V
		bool Draw();	// �`��

		bool ImageChange(const int nextKey);
	private:
		int _imageKey;	// �����p�L�[
		int _changeKey;	// �؂�ւ��p�L�[
		std::unordered_map<int, std::shared_ptr<Particle_Image>> _images;

		bool ImageClear();	// �o�^���̉��
		bool ImageInit();	// �摜�̏�����
		bool ChangeKey();	// �����p�L�[��؂�ւ��邩�H
	};
}

