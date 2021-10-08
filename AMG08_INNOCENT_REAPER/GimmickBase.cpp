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

	GimmickBase::GimmickBase(Game& game) : ObjectBase(game), _gType(GimmickType::LEVER) {
		_type = ObjectType::GIMMICK;
		_gType = GimmickType::LEVER;
	}

	GimmickBase::~GimmickBase() {
	}

	void GimmickBase::Init() {
	}

	void GimmickBase::Process() {
	}

	void GimmickBase::Draw() {
		// �`����W�̎Z�o
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		// �O���t�B�b�N�n���h���̎擾
		int graph;	
		GraphResearch(&graph);
		DrawRotaGraph(x, y, 1.0, 0, graph, true, _direction);
	}

	int GimmickBase::GimmickType() {
		// �M�~�b�N�̎�ނɉ������l��Ԃ�
		switch (_gType) {
		case GimmickType::LEVER:
			return gimmick::LEVER;
		case GimmickType::BLOCK:
			return gimmick::BLOCK;
		case GimmickType::CRYSTAL:
			return gimmick::CRYSTAL;
		case GimmickType::DOOR:
			return gimmick::DOOR;
		}
	}

	void GimmickBase::ObjValueUpdate() {
	}
}