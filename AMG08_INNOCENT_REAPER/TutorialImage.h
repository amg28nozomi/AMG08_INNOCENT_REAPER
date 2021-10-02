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
		bool _isCol;	// 当たり判定によって描画を行うか？
		AABB _collision;	// 当たり判定

		void IsCollision();
		void SetImage(ImageValue ivalue, int width = 0, int height = 0);	// イメージ情報
		void SetImage(ImageValue ivalue, int width1, int width2, int height1, int height2);	// イメージ情報
	};
}

