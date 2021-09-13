#include "Logo.h"
#include "ResourceServer.h"
#include "ObjectServer.h"
#include "Game.h"
#include "SoulCursor.h"
#include <DxLib.h>
#include <memory>

namespace {
	constexpr auto LOGO_FIRST = 0;
	constexpr auto LOGO_SECOND = 1;// SECOND
	constexpr auto LOGO_THIRD = 2; // third
	constexpr auto LOGO_FOURTH = 3;
	constexpr auto LOGO_MAX = 4;

	constexpr auto LUMINANCE = 255 / 60;

	constexpr auto WAIT_TIME = 120;	// �ҋ@����


	constexpr auto AMG_FADEIN = 60;//
	constexpr auto AMG_FADEOUT = 60;
	constexpr auto TEAM_FADEIN = 60;
	constexpr auto TEAM_FADEOUT = 60;

	constexpr auto ALLNUM_34 = 34;
	constexpr auto ALLNUM_30 = 30;

	constexpr auto TITLE_FRAME = 90;
	constexpr auto TITLE_ONE = TITLE_FRAME / 3;

	namespace image {
		constexpr auto FADEIN = true;
		constexpr auto FADEOUT = false;
	}
}

namespace inr {

	Logo::Logo(Game& game) : Image(game), _rgb(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND) {
		_tlogo = nullptr;
		Init();
	}

	void Logo::Init() {
		_wait = 0;
		_number = LOGO_FIRST;
		_calculation = rgb::ADD;	// ���Z�����ɐ؂�ւ�
		_animation = true;
		ChangeGraph();
	}

	void Logo::Process() {
		if (_tlogo != nullptr) _tlogo->Process();
		if (_animation) {
			// A�{�^���̓��͂��������ꍇ�͏������X�L�b�v����
			if (_number != LOGO_MAX && (_game.GetTrgKey() == PAD_INPUT_3 || CheckHitKey(KEY_INPUT_A) == TRUE)) {
				AnimaSkip();
			}
			// �ҋ@���Ԃ�����ꍇ�͌��Z�����������s���������甲����
			if (_wait) {
				--_wait;
				if (_wait == 0) {
					_fCount = 1;
					_rgb.Max();
				}
				return;
			}
			// ModeMain�œ��͏������������ꍇ�A�������X�L�b�v����悤�ɕύX����
			// �����ŕϐ��̒l��ύX
			++_fCount;

			switch(_calculation) {
			case true:
				if (_fCount > _maxFrame) {
					switch (_number) {
					case LOGO_MAX:
						_animation = false;
						_maxFrame = TITLE_FRAME;
						return;
					default:
						++_number;
						_calculation = false;
						_wait = WAIT_TIME;
						ChangeGraph();
						return;
					}
				}
				if (_number == LOGO_MAX) {
					_rgb.Update(true, LUMINANCE);
					break;
				}

			case false:
				if (_fCount > _maxFrame) {
					switch (_number) {
					case LOGO_FOURTH:
						_number = LOGO_MAX;
						_calculation = true;
						ChangeGraph();
						return;
					default:
						++_number;
						_calculation = true;
						_wait = 10;
						ChangeGraph();
						return;
					}
				}
			}
			return;
		}
		++_fCount;
		if (_fCount > _maxFrame) _fCount = 1;

	}

	void Logo::Draw() {
		int x = _pos.IntX();
		int y = _pos.IntY();

		auto interval = (static_cast<double>(_allnum) - 1) / (static_cast<double>(_maxFrame));
		int no = static_cast<int>(_fCount * interval);	// �������Z�o

		int grh = graph::ResourceServer::GetHandles(_graphKey, no);	// �`�悷��O���t�B�b�N�n���h���̎擾
		SetDrawBright(_rgb.Red(), _rgb.Green(), _rgb.Blue());
		DrawRotaGraph(x, y, 1.0, 0, grh, true, false);
		SetDrawBright(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND);
		if (_tlogo != nullptr) _tlogo->Draw();
	}

	void Logo::AnimationInit() {
		_graphKey = TEAM_LOGO;	// �`�[�����S�ɐ؂�ւ�
	}

	void Logo::ChangeGraph() {
		_fCount = 1;
		// ���݂̃i���o�[�ɉ����ăL�[��؂�ւ���
		switch (_number) {
		case LOGO_FIRST:	// AMG���S�t�F�[�h�C��
			_allnum = ALLNUM_34;
			_graphKey = titles::TITLE_ALI;
			_maxFrame = AMG_FADEIN;
			return;
		case LOGO_SECOND:	// AMG���S�t�F�[�h�A�E�g
			_allnum = ALLNUM_34;
			_graphKey = titles::TITLE_ALO;
			_maxFrame = AMG_FADEOUT;
			return;
		case LOGO_THIRD:	// �`�[�����S�t�F�[�h�C��
			_rgb.Min();
			_allnum = ALLNUM_30;
			_graphKey = titles::TITLE_TLI;
			_maxFrame = TEAM_FADEIN;
			return;
		case LOGO_FOURTH:	// �`�[�����S�t�F�[�h�A�E�g
			_graphKey = titles::TITLE_TLO;
			_maxFrame = TEAM_FADEOUT;
			return;
		case LOGO_MAX:	// �^�C�g�����S
			_rgb.Min();
			_graphKey = titles::TITLE_T;
			_maxFrame = 60;
			_tlogo = std::make_unique<TitleLogo>(_game.GetGame());
			_game.GetObjectServer()->Add(std::move(std::make_unique<SoulCursor>(_game.GetGame())));
			return;
		}
	}

	void Logo::AnimaSkip() {
		_wait = 0;
		_calculation = true;
		// ���݂�number�ɉ����ď����̃X�L�b�v���s��
		switch (_number) {
		case LOGO_FIRST:
			_wait = 10;
			_number = LOGO_THIRD;
			break;
		case LOGO_SECOND:
			_wait = 10;
			_number = LOGO_THIRD;
			break;
		case LOGO_THIRD:
			_number = LOGO_MAX;
			break;
		case LOGO_FOURTH:
			_number = LOGO_MAX;
			break;
		}
		ChangeGraph();
	}
}
