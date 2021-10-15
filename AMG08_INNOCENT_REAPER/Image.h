/*****************************************************************//**
 * \file   Image.h
 * \brief  æÌX[p[NX
 * 
 * \author éØóC
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>

namespace inr {

	namespace background {
		// wifÞÌeíîñ
		// óÌÕd
		constexpr auto BACK_GROUND_S = "back_groundS";					// æL[
		constexpr auto ALTAR = "back_altar";							// æL[(Oi)
		constexpr auto STAGES_HEIGHT = 2160;							// c 
		// rê½¬
		constexpr auto BACK_GROUND_1 = "back_ground1";					// æL[
		constexpr auto STAGE1_MAP_HEIGHT = 2160;						// c
		// _èÌX
		constexpr auto BACK_GROUND_2 = "back_ground2";					// æL[
		constexpr auto STAGE2_MAP_HEIGHT = 2160;						// c
		// {XXe[W
		constexpr auto BACK_GROUND_B = "back_groundB";					// æL[
	}
	namespace end {
		// X^bt[
		constexpr auto STAFF_ROLL = "staff_roll";						// æL[		
		constexpr auto STAFF_ROLL_WIDTH = 1920;							// ¡
		constexpr auto STAFF_ROLL_HEIGHT = 11880;						// c
	}
	// Aj[VfÞ
	// ^Cgwi
	constexpr auto BACK_GROUND = "backP";								// æL[
	constexpr auto BACK_GROUND_W = 1920;								// ¡
	constexpr auto BACK_GROUND_H = 2160;								// c
	// ^CgS
	constexpr auto TITLE_LOGO = "title_rogo";							// æL[
	constexpr auto TITLE_IMAGE_W = 1920;								// ¡
	constexpr auto TITLE_IMAGE_H = 1080;								// c
	// `[S
	constexpr auto TEAM_LOGO = "team_logo";								// æL[
	constexpr auto TEAM_IMAGE = 140;									// æTCY
	// ^Cgwi
	constexpr auto TITLE_BG = "title_back_ground";
	constexpr auto TITLE_IMAGE_WIDTH = 1920;
	constexpr auto TITLE_IMAGE_HEIGHT = 1080;
	// eí^CgUIÌæL[
	constexpr auto TITLE_START1 = "ui_start1";
	constexpr auto TITLE_START2 = "ui_start2";
	constexpr auto TITLE_EXIT1 = "ui_exit1";
	constexpr auto TITLE_EXIT2 = "ui_exit2";
	// ^CgUIÌTCY
	constexpr auto TITLE_UI_HEIGHT = 166;
	constexpr auto TITLE_START_WIDTH = 372;
	constexpr auto TITLE_EXIT_WIDTH = 298;

	class Game;		// ñdCN[hh~
	// æÌîêNX
	class Image {
	protected:
		Game& _game;			//@Q[QÆ
		Vector2 _pos;			// ÀW
		std::string _graphKey;	// æL[
	public:
		/**
		 * @brief				RXgN^
		 * @param game	Q[NXÌQÆ
		 */
		Image(Game& game);
		/**
		 * @brief	fXgN^
		 */
		~Image() = default;
		// ú»
		virtual void Init();
		// XV
		virtual void Process();
		// `æ
		virtual void Draw();
		// tOÌN®
		virtual void FlagOn();
	};
}

