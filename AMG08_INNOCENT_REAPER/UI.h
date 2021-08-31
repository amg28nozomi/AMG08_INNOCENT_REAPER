#pragma once
#include "Image.h"
#include <memory>
#include <vector>

namespace inr {

	namespace ui {
		constexpr auto KEY_HP = "player_my_soul";
		constexpr auto KEY_RED = "player_red_soul";
		constexpr auto KEY_BLUE = "player_blue_soul";

		constexpr auto HP_NUM = 5;
		constexpr auto HP_SIZE = 200;
	}

	class Player;
	class SoulSkin;

	class UI : public Image {
	public:
		UI(Game& game);
		~UI() = default;

		void Init() override;
		void Process() override;
		void Draw() override;
	private:
		std::shared_ptr<Player> _player;	// 自機のポインタ
		std::vector<std::shared_ptr<SoulSkin>> _pSouls;	// プレイヤーが所持する魂の有無
	};
}

