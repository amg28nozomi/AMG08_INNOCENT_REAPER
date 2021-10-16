#pragma once
#include "Particle_Image.h"
#include "AABB.h"

namespace inr {
	/** �`���[�g���A���e�L�X�g�̃R���p�C�����萔 */
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
	/** ��d�C���N���[�h�h�~ */
	class TutorialValue;
	/** �`���[�g���A���摜 */
	class TutorialImage : public Particle_Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		TutorialImage(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
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

