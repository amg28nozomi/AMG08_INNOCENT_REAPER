#pragma once
#include "Image.h"

namespace inr {
	class TitleLogo : public Image {
	public:
		TitleLogo(Game& game);
		
		void Init() override;
		void Draw() override;
	};
}

