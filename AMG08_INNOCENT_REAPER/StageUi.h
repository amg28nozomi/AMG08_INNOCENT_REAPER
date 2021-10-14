#pragma once
#include "Particle_Image.h"
#include <vector>

namespace inr {

	class StageUi : public Particle_Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		StageUi(Game& game);
		~StageUi() = default;

		void Init() override;
		void Process() override;
		void Draw() override;
		void SetParameter(std::string graph, Vector2 pos, int pal = 0);

		bool ChangeNumber(const std::string stageKey);	// �`�悷��A�C�R���؂�ւ�
		bool IntervalCount();	// �҂����Ԃ̃J�E���g����
		bool DrawStart() override;
		inline bool FadeDraw() { return _fadeDraw; }
	private:
		int _gNumber;	// ���Ԗڂ̉摜��`�悷�邩
		int _alive;	// �����t���[��
		bool _fadeDraw;	// �`�揈�������s������
	};

}

