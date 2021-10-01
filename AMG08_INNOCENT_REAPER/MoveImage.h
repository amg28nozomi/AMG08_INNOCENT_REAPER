#pragma once
#include "Image.h"

namespace inr {

	class MoveImage : public Image {
	public:
		MoveImage(Game& game);
		~MoveImage() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(const std::string gkey, Vector2 pos);	// ����1:�摜�p�L�[�A���������n�_

		inline void MoveOn() { if (_isMove == false) _isMove; }
	private:

		bool _isMove;	// �ړ��������s�����H
		Vector2 _start;	// �������W

		bool Move();
	};
}

