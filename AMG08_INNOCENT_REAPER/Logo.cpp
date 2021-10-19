/*****************************************************************//**
 * @file   Logo.cpp
 * @brief  ���S�N���X�i�摜�N���X�̃T�u�N���X�j
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "Logo.h"
#include "ResourceServer.h"
#include "ObjectServer.h"
#include "Game.h"
#include "SoulCursor.h"
#include <DxLib.h>
#include <memory>

namespace {
	// ���S�ԍ�
	constexpr auto LOGO_FIRST = 0;				// AMG���S�t�F�[�h�C��
	constexpr auto LOGO_SECOND = 1;				// AMG���S�t�F�[�h�A�E�g
	constexpr auto LOGO_THIRD = 2;				// �`�[�����S�t�F�[�h�C��
	constexpr auto LOGO_FOURTH = 3;				// �`�[�����S�t�F�[�h�A�E�g
	constexpr auto LOGO_MAX = 4;					// �^�C�g���A�j���[�V����

	constexpr auto LUMINANCE = 255 / 60;	
	constexpr auto WAIT_TIME = 120;				// �ҋ@����
	// �ő�t���[����
	constexpr auto AMG_FADEIN = 60;
	constexpr auto AMG_FADEOUT = 60;
	constexpr auto TEAM_FADEIN = 60;
	constexpr auto TEAM_FADEOUT = 60;
	constexpr auto TITLE_FRAME = 90;
	// ��������
	constexpr auto ALLNUM_34 = 34;
	constexpr auto ALLNUM_30 = 30;

	namespace image {
		// �A�j���[�V�����t���O
		constexpr auto FADEIN = true;
		constexpr auto FADEOUT = false;
	}
}

namespace inr {

	Logo::Logo(Game& game) : Image(game), _rgb(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND) {
		Init();		// ������
	}
	void Logo::Init() {
		// �e�평����
		_wait = 0;
		_number = LOGO_FIRST;
		_calculation = rgb::ADD;
		_animation = true;
		_tlogo = nullptr;
		ChangeGraph();
	}
	// �X�V
	void Logo::Process() {
		// �^�C�g�����S��null�ł͖����ꍇ�A�X�V�������Ăяo��
		if (_tlogo != nullptr) _tlogo->Process();
		// �A�j���[�V����
		if (_animation) {
			// A�{�^���̓��͂��������ꍇ�͏������X�L�b�v����
			if (_number != LOGO_MAX && (_game.GetTrgKey() == PAD_INPUT_3)) {
				AnimaSkip();		// �A�j���[�V�������X�L�b�v����
			}
			// �ҋ@���Ԃ�����ꍇ�͌��Z�����������s���������甲����
			if (_wait) {
				--_wait;				// ���Z�������s��
				if (_wait == 0) {
					_fCount = 1;	// �t���[���J�E���^
					_rgb.Max();		// RGB���ő�ɂ���
				}
				return;
			}
			++_fCount;		// �J�E���^�̉��Z
			// �t�F�[�h�C���E�t�F�[�h�A�E�g�̂ǂ���̏������s����
			switch(_calculation) {
			case true:		// �t�F�[�h�A�E�g
				// �J�E���^���ő�t���[�������̏ꍇ
				if (_fCount > _maxFrame) {
					switch (_number) {
					case LOGO_MAX:	// �^�C�g���A�j���[�V�����̏ꍇ
						_animation = false;				// �A�j���[�V�����I��
						_maxFrame = TITLE_FRAME;	// �ő�t���[���ݒ�
						return;
					default:
						++_number;								// �����ԍ����Z
						_calculation = false;			// �t�F�[�h�C���ɑJ��
						_wait = WAIT_TIME;				// �ҋ@���Ԑݒ�
						ChangeGraph();						// ���S�摜�؂�ւ�
						return;
					}
				}
				// �J�E���^���ő傩�����ԍ����ő�̏ꍇ
				if (_number == LOGO_MAX) {
					_rgb.Update(true, LUMINANCE);	// RGB���X�V����
					break;
				}
			case false:		// �t�F�[�h�C��
				// �J�E���^���ő�t���[�������̏ꍇ
				if (_fCount > _maxFrame) {
					switch (_number) {
					case LOGO_FOURTH:	// �`�[�����S�t�F�[�h�A�E�g�̏ꍇ
						_number = LOGO_MAX;		// �����ԍ����ő�ɂ���
						_calculation = true;	// �t�F�[�h�C���J�n
						ChangeGraph();				// ���S�摜�؂�ւ�
						return;
					default:
						++_number;						// �����ԍ����Z
						_calculation = true;	// �t�F�[�h�C���ɑJ��
						_wait = 10;						// �ҋ@���Ԑݒ�
						ChangeGraph();			// ���S�摜�؂�ւ�
						return;
					}
				}
			}
			return;
		}
		++_fCount;	// �J�E���^���Z
		if (_maxFrame < _fCount) _fCount = 1;	// �ő�l�𒴂����ꍇ�͏C������
	}

	void Logo::Draw() {
		// �`����W
		int x = _pos.IntX();
		int y = _pos.IntY();
		// �O���t�B�b�N�n���h���̎擾
		auto interval = (static_cast<double>(_allnum) - 1) / (static_cast<double>(_maxFrame));
		int no = static_cast<int>(_fCount * interval);
		int grh = graph::ResourceServer::GetHandles(_graphKey, no);
		SetDrawBright(_rgb.Red(), _rgb.Green(), _rgb.Blue());						// �P�x�ݒ�
		DrawRotaGraph(x, y, 1.0, 0, grh, true, false);									// �`��
		SetDrawBright(rgb::MAX_BLEND, rgb::MAX_BLEND, rgb::MAX_BLEND);	// �P�x������
		if (_tlogo != nullptr) _tlogo->Draw();													// �|�C���^������ꍇ�͕`�揈���Ăяo��
	}

	void Logo::AnimationInit() {
		_graphKey = TEAM_LOGO;		// �`�[�����S�ɐ؂�ւ�
	}

	void Logo::ChangeGraph() {
		_fCount = 1;	// �J�E���^������
		// ���݂̏����ԍ��ɉ����ăL�[����ђl��؂�ւ���
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
		case LOGO_MAX:		// �^�C�g�����S
			_rgb.Min();
			_graphKey = titles::TITLE_T;
			_maxFrame = 60;
			// �^�C�g�����S�̐�������ѓo�^
			_tlogo = std::make_unique<TitleLogo>(_game.GetGame());
			_game.GetObjectServer()->Add(std::move(std::make_unique<SoulCursor>(_game.GetGame())));
			return;
		}
	}

	void Logo::AnimaSkip() {
		_wait = 0;						// �҂����Ԃ̏�����
		_calculation = true;	// �t�F�[�h�A�E�g�ɑJ��
		// ���݂̏����ԍ��ɉ����ď����̃X�L�b�v���s��
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
		ChangeGraph();				// ���S�摜�̐؂�ւ�
	}
}
