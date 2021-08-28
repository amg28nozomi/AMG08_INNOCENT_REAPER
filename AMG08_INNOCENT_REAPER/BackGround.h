#pragma once
#include "Image.h"
#include "Vector2.h"
#include <vector>

namespace inr {

	class BackGround : public Image{
	public:
		BackGround(Game& game);
		
		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		std::pair<std::vector<Vector2>, std::vector<Vector2>> _positions;	// �`����W(����:1���ڂ̕`����W, �E��2���ڂ̕`����W)
		std::vector<double> _scrSpeed;	// �X�N���[���ړ���
	};
}

