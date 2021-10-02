#pragma once
#include "Image.h"
#include "ImageValue.h"
#include <vector>

namespace inr {

	namespace fgd {
		constexpr auto FORE_STAGE0 = "fore_stage0";
		constexpr auto FORE_STAGET = "fore_stageT";
		constexpr auto FORE_STAGE1 = "fore_stage1";
		constexpr auto FORE_STAGE2 = "fore_stage2";
		constexpr auto FORE_STAGE2_1 = "fore_stage2_1";
		constexpr auto FORE_STAGE2_2 = "fore_stage2_2";
		constexpr auto FORE_STAGE3 = "fore_stageb";

		constexpr auto FORE_OBJ = "fore_stage0_obj";
		constexpr auto FORE_OBJ_W = 1920;
		constexpr auto FORE_OBJ_H = 2160;

		constexpr auto FORE0_W = 3840;
		constexpr auto FORE0_H = 2160;

		constexpr auto FORET_W = 5760;
		constexpr auto FORET_H = 1080;

		constexpr auto FORE1_W = 9600;
		constexpr auto FORE1_H = 2160;
		constexpr auto FORE2_W = 9600;
		constexpr auto FORE2_H = 2160;
		constexpr auto FORE2_1_W = 9600;
		constexpr auto FORE2_1_H = 2160;
		constexpr auto FORE2_2_W = 3840;
		constexpr auto FORE2_2_H = 3240;
		constexpr auto FORE3_W = 5760;
		constexpr auto FORE3_H = 1080;
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
		bool _draw;	// í«â¡ï`âÊ

		int CheckKey(const std::string key);
		void BackDraw();
	};
}

