#pragma once
#include "Image.h"

namespace inr {

	class MoveImage : public Image {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		MoveImage(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~MoveImage() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(const std::string gkey, Vector2 pos);	// ����1:�摜�p�L�[�A���������n�_

		inline void MoveOn() { if (_isMove == false) _isMove; }
		inline bool IsActive() { return _isMove; }
	private:

		bool _isMove;	// �ړ��������s�����H
		Vector2 _start;	// �������W

		bool Move();
	};
}

