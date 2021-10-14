#pragma once
#include "Particle_Image.h"
#include "AABB.h"

namespace inr {

	namespace tutorial {
		constexpr auto ROB = "titorial_rob";
		constexpr auto GIVE = "titorial_give";
		constexpr auto DASH = "titorial_dash";
		constexpr auto JUMP = "titorial_jump";
		constexpr auto GRAB = "titorial_grab";
		constexpr auto INTERACTION = "titorial_interaction";
		constexpr auto CRYSTAL = "titorial_crystal";
		constexpr auto LEVER = "titorial_lever";
		constexpr auto EMPTY = "titorial_empty";
	}

	class TutorialValue;

	class TutorialImage : public Particle_Image {
	public:
		TutorialImage(Game& game);
		~TutorialImage() = default;

		// void Init() override;
		void Process() override;
		void Draw() override;

		void SetTutorialImage(TutorialValue tvalue);	// �C���[�W���

	private:
		bool _isCol;	// �����蔻��ɂ���ĕ`����s�����H
		AABB _collision;	// �����蔻��

		void IsCollision();
	};
}

