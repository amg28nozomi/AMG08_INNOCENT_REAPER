#pragma once
#include "Image.h"

namespace inr {

	namespace image {
		namespace particle {
			constexpr auto BG_BLACK = "bg_black1";
			constexpr auto BG_OPERATION = "oparation";
			constexpr auto BG_SIZE_W = 1920;
			constexpr auto BG_SIZE_H = 1080;

			constexpr auto CONTINUE = "continue";
			constexpr auto CONTROLS = "controls";// controller
			constexpr auto QUIT_TO_TITLE = "quit_to_title";

			constexpr auto CONTINUE_W = 272;
			constexpr auto CONTROLS_W = 284;
			constexpr auto QUIT_TO_TITLE_W = 365;
			constexpr auto UI_H_1 = 55;
			constexpr auto UI_H_2 = 64;

			constexpr auto CURSOR = "cursor";	// Cursor
			constexpr auto CURSOR_W = 500;
			constexpr auto CURSOR_H = 150;

			constexpr auto STAGE_UI = "ui_stage";
			constexpr auto STAGE_UI_W = 692;
			constexpr auto STAGE_UI_H = 279;

			constexpr auto NORMAL = false;
			constexpr auto UI = true;
		}
	}

	namespace animation {
		constexpr auto A_SUB = -1;
		constexpr auto A_NORMAL = 0;
		constexpr auto A_ADD = 1;
	}

	class ImageValue;

	class Particle_Image : public Image {
	protected:
		int _animation;	// アニメーションを行うか
		int _pal;	// 透明値
		double _maxExt;		// 最大拡大率
		double _extRate;	// 拡大率
		bool _isDraw;	// 描画処理を行っているか
		bool _imageType;
		bool _end;	// 処理を終了するか
		Vector2 _startPos;

		bool AddPal();
		bool SubPal();
	public:
		Particle_Image(Game& game);
		~Particle_Image() = default;

		void Init() override;
		virtual void Process() override;
		void Draw() override;

		void SetParameter(std::string graph, Vector2 pos, int pal = 0, double extrate = 1.0);
		void SetImage(ImageValue ivalue);
		bool Animation();
		virtual bool DrawStart();
		bool DrawEnd();
		bool End();

		inline bool IsType() { return _imageType; }
		inline bool IsDraw() { return _isDraw; }
		bool IsNormal();
		virtual void ChangePosition(bool type);
	};
}

