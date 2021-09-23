#pragma once
#include "Image.h"
#include "ImageValue.h"
#include <vector>

namespace inr {

	namespace fgd {
		constexpr auto FORE_STAGE0 = "fore_stage0";
		constexpr auto FORE_STAGE1 = "fore_stage1";
		constexpr auto FORE_STAGE2 = "fore_stage2";
		constexpr auto FORE_STAGE2_1 = "fore_stage2_1";
		constexpr auto FORE_STAGE2_2 = "fore_stage2_2";
		constexpr auto FORE_STAGE3 = "fore_stageb";

		constexpr auto FORE0_W = 3840;
		constexpr auto FORE0_H = 2160;
		constexpr auto FORE1_W = 9600;
		constexpr auto FORE1_H = 2160;
		constexpr auto FORE2_W = 0;
		constexpr auto FORE2_H = 0;
	}

	class ForeGround : public Image {
	public:
		ForeGround(Game& game);
		~ForeGround();

		void Init() override;
		void Draw() override;

		bool SetForeGround(std::vector<ImageValue> ive);
		bool SetKey(const std::string key);	// åªç›ÇÃÉLÅ[ÇêÿÇËë÷Ç¶ÇÈ

	private:
		std::vector<ImageValue> _ivalue;

		int CheckKey(const std::string key);
	};
}

