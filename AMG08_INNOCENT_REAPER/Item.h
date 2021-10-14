/*****************************************************************//**
 * \file   Item.h
 * \brief  �A�C�e���N���X
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>
#include "Particle_Image.h"
#include "Collision.h"
#include "ObjectValue.h"

namespace inr {
	namespace item {
		// �A�C�e��
		constexpr auto ITEM = "item_light";		// �L�[
		constexpr auto ITEM_IMAGE = 150;		// �摜�T�C�Y
	}
	// ��d�C���N���[�h�h�~
	class Game;
	// �A�C�e���N���X
	class Item {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		Item(Game& game);
		// �f�X�g���N�^
		~Item() = default;
		// ������
		void Init();
		// �X�V
		void Process();
		// �`��
		void Draw();
		// �I�u�W�F�N�g���̓o�^
		void SetParameter(ObjectValue ovalue);
		// �I�u�W�F�N�g���̍X�V
		void ObjValueUpdate();
		// �I�u�W�F�N�g�͊��ɓ��肳��Ă��邩
		inline bool IsGet() { return _isGet; }
		// �����t���O�̎擾
		inline bool IsDel() { return _del; }
		// �I�u�W�F�N�g���̎擾
		inline ObjectValue GetObjectValue() { return _oValue; }
	private:
		Game& _game;			// �Q�[���Q��
		ObjectValue _oValue;	// �I�u�W�F�N�g���
		Collision _col;			// �����蔻��
		std::string _gkey;		// �摜�L�[
		Vector2 _position;		// ���W
		int _count;				// �J�E���^
		int _maxFrame;			// �A�j���[�V�����̕`��t���[����
		int _messageNo;			//�@�Ή����Ă���message�ԍ�
		bool _isGet;			// ���łɓ��肳��Ă��邩�H
		bool _del;				// �����t���O
	};

}
