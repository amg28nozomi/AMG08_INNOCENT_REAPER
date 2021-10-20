/*****************************************************************//**
 * @file   FadeBlack.cpp
 * @brief  �t�F�[�h�C���E�t�F�[�h�A�E�g���s���t�F�[�h�u���b�N�N���X�i�摜�N���X�̃T�u�N���X�j
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "FadeBlack.h"
#include "ResourceServer.h"
#include <DxLib.h>

namespace {
	constexpr auto LUMINANCE = 255 / 60;
	constexpr auto FADE_VALUE = 255 / 60;
}

namespace inr {

	FadeBlack::FadeBlack(Game& game) : Image(game) {
		_pal = 0;
		_type = image::FADE_OUT;
		Init();
	}

	FadeBlack::~FadeBlack() {
	}

	void FadeBlack::Init() {
		// �e�평����
		_end = true;
		_interval = 0;
		_isInterval = false;
		_addEnd = true;
	}

	void FadeBlack::Process() {
		if (_end == true) return;							// �������Ȃ��ꍇ�͍X�V�I��
		if (_addEnd == true) _addEnd = false;	// �t���O������ꍇ�͋U�ɂ���
		// �P�\���Ԕ��肪����ꍇ
		if (_isInterval) {
			// �P�\���Ԃ�0�ɂȂ����ꍇ
			if (_interval == 0) {
				_isInterval = false;				// �t���O���I������
				_type = image::FADE_IN;			// �t�F�[�h�C�����J�n����
				return;
			}
			--_interval;		// �P�\���Ԃ����炷
			return;
		}
		// �t���O�ɉ����ď������s��
		switch (_type) {
		case image::FADE_IN:
			FadeIn();			// �t�F�[�h�C��
			return;
		case image::FADE_OUT:
			FadeOut();		// �t�F�[�h�A�E�g
			return;
		}
	}

	void FadeBlack::Draw() {
		if (_end == true) return;	// �����͍s��Ȃ�
		// �摜�̎擾
		auto graph = graph::ResourceServer::GetHandles(image::BLACK, 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _pal);	
		DrawRotaGraph(961, 540, 1.0, 0, graph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void FadeBlack::FlagChange(bool fadeType, int interval) { 
		// �e����
		_type = fadeType;
		_end = false;
		_interval = interval;
	}

	void FadeBlack::FadeIn() {
		_pal -= FADE_VALUE;		// �����x�̍X�V
		FadeEnd();						// �������I������̂��̔���
	}

	void FadeBlack::FadeOut() {
		_pal += FADE_VALUE;		// �����̍X�V
		FadeEnd();						// �������I�����邩�̔���
	}

	bool FadeBlack::FadeEnd() {
		// �ǂ���̏������s���Ă��邩
		switch (_type) {
			// �t�F�[�h�C��
		case image::FADE_IN:
			if (0 < _pal) return false;		// �����x�������ɂȂ��Ă��Ȃ�
			if (_pal < 0) _pal = 0;				// ���������ɂȂ����ꍇ�͒l���C������
			_end = true;									// �����I��
			_isInterval = false;					// �P�\�t���O���I�t�ɂ���
			return true;
			// �t�F�[�h�A�E�g
		case image::FADE_OUT:
			if (_pal < 255) return false;	// �����x������ɂȂ��Ă��Ȃ�
			if (255 < _pal) _pal = 255;		// ����𒴉߂����ꍇ�͒l���C������
			_addEnd = true;								// ���Z�����I��
			break;
		}
		_isInterval = true;		// �P�\�������s��
		return true;
	}
}