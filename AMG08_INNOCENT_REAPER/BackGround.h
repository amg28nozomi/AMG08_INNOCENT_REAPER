#pragma once
#include "Image.h"
#include "Vector2.h"

namespace inr {

	class BackGround : public Image{
	public:
		BackGround(Game& game);
		
		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		Vector2 _pos2;
	};
}

