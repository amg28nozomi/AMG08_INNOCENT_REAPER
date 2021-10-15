/*****************************************************************//**
 * \file   GimmickBase.cpp
 * \brief  �M�~�b�N�̃X�[�p�[�N���X�i�I�u�W�F�N�g�x�[�X�̃T�u�N���X�j
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#include "GimmickBase.h"
#include "Game.h"

namespace inr {
	// �R���X�g���N�^
	GimmickBase::GimmickBase(Game& game) : ObjectBase(game), _gType(GimmickType::LEVER) {
		// ������
		_type = ObjectType::GIMMICK;
		_gType = GimmickType::LEVER;
	}
	// �f�X�g���N�^
	GimmickBase::~GimmickBase() {
	}
	// ������
	void GimmickBase::Init() {
	}
	// �X�V
	void GimmickBase::Process() {
	}
	// �`��
	void GimmickBase::Draw() {
		// �`����W�̎Z�o
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// �O���t�B�b�N�n���h���̎擾
		int graph;	
		GraphResearch(&graph);
		// �`��
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);
	}
	// �M�~�b�N�ԍ��̎擾
	int GimmickBase::GimmickType() {
		// �M�~�b�N�̎�ނɉ������l��Ԃ�
		switch (_gType) {
		case GimmickType::LEVER:
			return gimmick::LEVER;		// ���o�[
		case GimmickType::BLOCK:
			return gimmick::BLOCK;		// �����
		case GimmickType::CRYSTAL:
			return gimmick::CRYSTAL;	// ����
		case GimmickType::DOOR:
			return gimmick::DOOR;			// �h�A
		}
	}
	// �I�u�W�F�N�g���̍X�V
	void GimmickBase::ObjValueUpdate() {
	}
}