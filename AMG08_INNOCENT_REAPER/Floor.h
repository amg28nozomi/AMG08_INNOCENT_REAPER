#ifdef _DEBUG
#pragma once
#include "Vector2.h"

class Vector2;

// ���̑���B
namespace inr {

	class Game;

	class Floor {
	public:
		Floor(Game& game);
		~Floor();
		void Init();	// ������
		void Process();	// �X�V
		void Draw();	// �`��
	private:
		Game& _game;
		Vector2 _pos;	//��
	};

}

#endif