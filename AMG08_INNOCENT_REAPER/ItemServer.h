/*****************************************************************//**
 * \file   ItemServer.h
 * \brief  �A�C�e���T�[�o�N���X
 *		   �A�C�e���̊Ǘ����s��
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
	// ��d�C���N���[�h�h�~
	class Item;
	// �A�C�e���T�[�o
	class ItemServer {
	public:
		// �R���X�g���N�^
		ItemServer();
		// �f�X�g���N�^
		~ItemServer();
		// �`��
		void Process();
		// �X�V
		void Draw();
		// �A�C�e���̓o�^
		void Add(std::shared_ptr<Item> item);
		// �R���e�i�̉��
		void ItemClear();
		// �����t���O�̋N��
		bool DelOn();
		// �A�C�e���R���e�i�̎擾
		std::vector<std::shared_ptr<Item>> GetItems() { return _items; }
	private:
		bool _isDel;								// �I�u�W�F�N�g�̏������s����
		std::vector<std::shared_ptr<Item>> _items;	// �A�C�e���Ǘ��p�̃R���e�i
		// �A�C�e���̏���
		bool Delete();
	};
}

