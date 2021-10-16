/*****************************************************************//**
 * @file   ItemServer.cpp
 * @brief  �A�C�e���̊Ǘ����s���A�C�e���T�[�o�N���X
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#include "ItemServer.h"
#include "Item.h"
#include <algorithm>

namespace inr {
	// �R���X�g���N�^
	ItemServer::ItemServer() {
		ItemClear();	// �R���e�i�̉��
	}
	// �f�X�g���N�^
	ItemServer::~ItemServer() {
		ItemClear();	// �R���e�i�̉��
	}
	// �R���e�i�̉��
	void ItemServer::ItemClear() {
		_isDel = false;
		_items.clear();
	}
	// �A�C�e���̓o�^
	void ItemServer::Add(std::shared_ptr<Item> item) {
		_items.emplace_back(item);
	}
	// �X�V
	void ItemServer::Process() {
		Delete();			// ��������
		for (auto item : _items) item->Process();	// �A�C�e���̍X�V�����Ăяo��
	}
	// �`��
	void ItemServer::Draw() {
		for (auto item : _items) item->Draw();		// �A�C�e���̕`�揈���Ăяo��
	}
	// ����
	bool ItemServer::Delete() {
		if (_isDel == false) return false;	// �������Ȃ�
		_isDel = false;											// �t���O�I�t
	}
	// �����t���O�̋N��
	bool ItemServer::DelOn() {
		if (_isDel == true) return false;	// ���Ƀt���O���I���̏ꍇ�͏������I��
		_isDel = true;										// �����t���O�N��
		return true;
	}
}