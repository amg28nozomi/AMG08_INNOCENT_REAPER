/*****************************************************************//**
 * @file   ItemServer.h
 * @brief  �A�C�e���̊Ǘ����s���A�C�e���T�[�o�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class Item;
	/** �A�C�e���T�[�o */
	class ItemServer {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 */
		ItemServer();
		/**
		 * @brief				�f�X�g���N�^
		 */
		~ItemServer();
		/**
		 * @brief				�X�V����
		 */
		void Process();
		/**
		 * @brief				�`�揈��
		 */
		void Draw();
		/**
		 * @brief				�A�C�e���̓o�^
		 * @param item	�o�^����A�C�e���̃V�F�A�[�h�|�C���^
		 */
		void Add(std::shared_ptr<Item> item);
		/**
		 * @brief				�A�C�e���̊J��
		 */
		void ItemClear();
		/**
		 * @brief				�����t���O�̋N��
		 */
		bool DelOn();
		/**
		 * @brief				�S�A�C�e���̎擾
		 * @return			�A�C�e����o�^���Ă���R���e�i��Ԃ�
		 */
		std::vector<std::shared_ptr<Item>> GetItems() { return _items; }
	private:
		bool _isDel;																//!< �I�u�W�F�N�g�̏������s����
		std::vector<std::shared_ptr<Item>> _items;	//!< �A�C�e���Ǘ��p�̃R���e�i
		/**
		 * @brief				�A�C�e���̏���
		 * @return			���������ꍇ��true��Ԃ�
		 *							���s�����ꍇ��false��Ԃ�
		 */
		bool Delete();
	};
}

