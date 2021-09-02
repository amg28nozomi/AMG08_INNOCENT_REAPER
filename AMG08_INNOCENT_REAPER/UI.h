#pragma once
#include "Image.h"
#include <memory>
#include <vector>
#include <queue>

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
		std::shared_ptr<Player> _player;	// ���@�̃|�C���^
		std::queue<std::shared_ptr<SoulSkin>> _uiSoul;
		std::vector<std::string> _ghKeys;
		std::vector<int> _count;
		std::vector<bool> _active;	// �Ώۂ͊��������Ă��邩�H

		std::string GetGraphKey(int number);
		int GraphHandle(const std::string key, int count);
		void SoulResearch();
		int IsSoulChange(bool value);
		void Dels();	// �J�E���^������ɓ��B�����ꍇ�A��������
		void ActiveCount();
	};
}

