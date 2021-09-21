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
		_player = nullptr;
		_pos = { 50, 100 };
		_ghKeys = { ui::KEY_HP };
		_count = { 0 };
		_pal = { 255 };
		_active = { true, true };
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
		GraphUpdata();
		// �X�V����
		// ���@���ۗL���鍰�̐����ς�����ꍇ�̂ݍX�V��������
		// �Y�����鍰���Ȃ��Ȃ����ꍇ�A�j���[�V�������J�n����
	}

	void UI::Draw() {
		DrawEmptyBox();
		for (auto number = 0; number < _count.size(); ++number) {
			auto gh = GraphHandle(_ghKeys[number], _count[number]);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal[number]);
			DrawRotaGraph(_pos.IntX() + ((number + 1) * 100), _pos.IntY(), 1.0, 0, gh, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	void UI::DrawEmptyBox() {
		auto gh = GraphHandle(ui::KEY_BOX, 0);
		DrawRotaGraph(300, 100, 1.0, 0, gh, true);
	}

	void UI::PlayerUpdate() {
		if (_player == nullptr) _player = _game.GetObjectServer()->GetPlayer();
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

	void UI::GraphUpdata() {
		if (_uiSoul.empty()) return;
		auto it = _uiSoul;
		for (auto number = 1; number < static_cast<int>(_ghKeys.size()); ++number) {
			if (_uiSoul.size() < number) break;
			if (_active.at(number) == false) continue;
			_ghKeys.at(number) = GetGraphKey(it.front()->SoulColor());
			it.pop();
		}

		// �P�x���}�b�N�X�ł͂Ȃ��ꍇ
		for (auto&& pal : _pal) {
			if (pal == 255) continue;
			if (255 < pal) {
				pal = 255;
				continue;
			}
			pal += 5;
		}
	}

	int UI::GraphHandle(const std::string key, int count) {
		// �O���t�B�b�N�n���h���̎Z�o
		auto interval = 3;
		int no = static_cast<int>(count / interval) % 25;	// �������Z�o;
		return graph::ResourceServer::GetHandles(key, no);
	}

	// �A�j���[�V�������I�����Ă��獰���������悤�ɂ���
	// �����O�ȏ�o�^����Ă���ꍇ�͍X�V���s��Ȃ�

	void UI::SoulResearch() {
		auto ps = _player->GetSouls();
		// �T�C�Y���Ⴄ�ꍇ�͍X�V��������
		if (_uiSoul.size() != ps.size()) {
			// ���@���V����������肵���ꍇ
			if (_uiSoul.size() < ps.size()) {
				auto changes = IsSoulChange(ADD_SOUL);
				auto it = ps;
				auto loopMax = static_cast<int>(ps.size() - changes);
				for (int i = 0; i < loopMax; ++i) it.pop();	// �d������v�f������

				
				for (auto i = 0; i < changes; ++i) {
					_ghKeys.emplace_back(GetGraphKey(static_cast<int>(it.back()->SoulColor())));	// �����Ŏ~�܂�
					_count.emplace_back(0);
					_active.emplace_back(true);
					_pal.emplace_back(0);
					it.pop();
				}
				_uiSoul = ps;
				return;
			}
			// ���@�̍����������ꍇ
			if (ps.size() < _uiSoul.size()) {
				auto changes = IsSoulChange(SUB_SOUL);
				for (auto i = 0; i < changes; ++i) {
					_active[i + 1] = false;	// �񊈐���Ԃɂ���
				}
				_uiSoul = ps;
				return;
			}
		}
		_uiSoul = ps;
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
			if (static_cast<int>(_count.size()) <= i) break;
			int no;
			if (usize != _count.size()) { 
				no = (static_cast<int>(_count.size()) - usize) * -1;
			}
			else no = i;

			if (_count[no] < 75) continue;
			_ghKeys.erase(_ghKeys.begin() + 1);
			_count.erase(_count.begin() + 1);
			_active.erase(_active.begin() + 1);
			_pal.erase(_pal.begin() + 1);
		}
	}

	void UI::ActiveCount() {
		for (auto i = 0; i < _active.size(); ++i) {
			if (_active[i] == true) continue;
			++_count[i];
		}
	}
}
