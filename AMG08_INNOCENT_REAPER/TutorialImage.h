#pragma once
#include "Particle_Image.h"
#include "Collision.h"

namespace inr {
	class TutorialImage : public Particle_Image {
	public:
		TutorialImage(Game& game);
		~TutorialImage() = default;

		// void Init() override;
		void Process() override;
		void Draw() override;

	private:
		bool _isCol;	// �����蔻��ɂ���ĕ`����s�����H
		AABB _collision;	// �����蔻��

		void IsCollision();
		void SetImage(ImageValue ivalue, int width = 0, int height = 0);	// �C���[�W���
		void SetImage(ImageValue ivalue, int width1, int width2, int height1, int height2);	// �C���[�W���
	};
}

