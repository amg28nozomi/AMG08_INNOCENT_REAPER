#include "UI.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ResourceServer.h"
#include "Player.h"
#include "SoulSkin.h"

namespace {
	constexpr auto UI_SPACE_W = 100;

	constexpr auto NUMBER_HP = -1;
	constexpr auto NUMBER_RED = 0;
	constexpr auto NUMBER_BLUE = 1;

	constexpr auto SUB_SOUL = false;
	constexpr auto ADD_SOUL = true;

	constexpr auto SOUL_ANIMA_MAX = 75;
}

namespace inr {

	UI::UI(Game& game) : Image(game) {
		_player = _game.GetObjectServer()->GetPlayer();
		_pos = { 300, 200 };
		_ghKeys = { ui::KEY_HP };
		_count = { 0 };
		_active = { true };
		_uiSoul;
		Init();
	}

	void UI::Init() {
		// �e�폈���̏�����
	}

	void UI::Process() {
		SoulResearch();
		ActiveCount();

		Dels();
		// �X�V����
		// ���@���ۗL���鍰�̐����ς�����ꍇ�̂ݍX�V��������
		// �Y�����鍰���Ȃ��Ȃ����ꍇ�A�j���[�V�������J�n����
	}

	void UI::Draw() {
		for (auto number = 0; number < _count.size(); ++number) {
			auto gh = GraphHandle(_ghKeys[number], _count[number]);
			DrawRotaGraph(_pos.IntX() * (number + 1), _pos.IntY(), 1.0, 0, gh, true);
		}
	}

	std::string UI::GetGraphKey(int number) {
		// �Ԃ��Ă����ԍ��ɉ������L�[��Ԃ�
		switch (number) {
		case NUMBER_RED:
			return ui::KEY_RED;
		case NUMBER_BLUE:
			return ui::KEY_BLUE;
		default:
			return ui::KEY_HP;
		}
	}

	int UI::GraphHandle(const std::string key, int count) {
		// �O���t�B�b�N�n���h���̎Z�o
		auto interval = 3;
		int no = static_cast<int>(count / interval) % 25;	// �������Z�o;
		return graph::ResourceServer::GetHandles(key, no);
	}

	void UI::SoulResearch() {
		auto ps = _player->GetSouls();
		// �T�C�Y���Ⴄ�ꍇ�͍X�V��������
		if (_uiSoul.size() != ps.size()) {
			// ���@���V����������肵���ꍇ
			if (_uiSoul.size() < ps.size()) {
				auto changes = IsSoulChange(ADD_SOUL);
				auto loopMax = static_cast<int>(ps.size() - changes);
				for (int i = 0; i < loopMax; ++i) ps.pop();	// �d������v�f������

				for (auto i = 0; i < changes; ++i) {
					_uiSoul.push(ps.front());
					_ghKeys.emplace_back(GetGraphKey(static_cast<int>(_uiSoul.back()->SoulColor())));
					_count.emplace_back(0);
					_active.emplace_back(true);
					ps.pop();
				}
				return;
			}
			// ���@�̍����������ꍇ
			if (ps.size() < _uiSoul.size()) {
				auto changes = IsSoulChange(SUB_SOUL);
				for (auto i = 0; i < changes; ++i) {
					_uiSoul.pop();
					_active[i + 1] = false;	// �񊈐���Ԃɂ���
				}

			}
		}
	}

	int UI::IsSoulChange(bool value) {
		switch (value) {
		case ADD_SOUL:
			return static_cast<int>(_player->GetSouls().size() - _uiSoul.size());
		case SUB_SOUL:
			return static_cast<int>(_uiSoul.size() - _player->GetSouls().size());
		}
	}

	void UI::Dels() {
		auto usize = static_cast<int>(_count.size());
		for (auto i = 0; i < usize; ++i) {
			if (i == 0)continue;
			if (_count[i] < 24) continue;
			_ghKeys.erase(_ghKeys.begin() + i);
			_count.erase(_count.begin() + i);
			_active.erase(_active.begin() + i);
		}
	}

	void UI::ActiveCount() {
		for (auto i = 0; i < _active.size(); ++i) {
			if (_active[i] == true) continue;
			++_count[i];
		}
	}
}
