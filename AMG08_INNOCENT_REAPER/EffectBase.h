/*****************************************************************//**
 * @file   EffectBase.h
 * @brief  �G�t�F�N�g�̃X�[�p�[�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>

namespace inr {
	/** �G�t�F�N�g */
	namespace effect {
		// �G�t�F�N�g�̉摜�L�[��
		constexpr auto JUMP = "jump_effect";						//!< ���@�W�����v�G�t�F�N�g
		constexpr auto DASH = "dash_effect";						//!< ���@�_�b�V���G�t�F�N�g
		constexpr auto ROB = "rob_effect";							//!< ���@�D���G�t�F�N�g
		constexpr auto HIT = "hit_effect";							//!< ���@�_���[�W�G�t�F�N�g
		constexpr auto DEAD = "dead_effect";						//!< ���@���S�G�t�F�N�g
		constexpr auto GIVE = "give_effect";						//!< ���@�^����G�t�F�N�g
		constexpr auto DEBUFF = "playe_debuff_effect";	//!< ���@�f�o�t�G�t�F�N�g
		// �W�����v�G�t�F�N�g�̉摜�T�C�Y
		constexpr auto JUMP_IMAGE_W = 150;							//!< ���@�W�����v�G�t�F�N�g�̕�
		constexpr auto JUMP_IMAGE_H = 50;								//!< ���@�W�����v�G�t�F�N�g�̍���
		// �D���G�t�F�N�g�̉摜�T�C�Y
		constexpr auto ROB_IMAGE = 140;									//!< ���@�D���G�t�F�N�g�̃T�C�Y
		// �_���[�W�G�t�F�N�g�̉摜�T�C�Y
		constexpr auto HIT_IMAGE_W = 840;								//!< �_���[�W�G�t�F�N�g�̕�
		constexpr auto HIT_IMAGE_H = 430;								//!< �_���[�W�G�t�F�N�g�̍���
		// �^����G�t�F�N�g�̉摜�T�C�Y
		constexpr auto GIVE_IMAGE = 200;								//!< �^����G�t�F�N�g�̃T�C�Y
		// �f�o�t�G�t�F�N�g�̉摜�T�C�Y����ё�������
		constexpr auto DEBUF_IMAGE = 140;								//!< �f�o�t�G�t�F�N�g�̃T�C�Y
		constexpr auto DEBUF_MAX = 15;									//!< �f�o�t�G�t�F�N�g�̑�������
		/** �G�̃R���p�C�����萔 */
		namespace enemy {
			constexpr auto S_HIT = "soul_hit_effect";			//!< ���S�G�t�F�N�g
			constexpr auto SHIT_IMAGE = 400;							//!< ���S�G�t�F�N�g�̃T�C�Y
			constexpr auto HITDROP = "hipdrop_effect";		//!< �q�b�v�h���b�v�G�t�F�N�g
			constexpr auto HIPDROP_WIDTH = 560;						//!< �q�b�v�h���b�v�G�t�F�N�g�̕�
			constexpr auto HIPDROP_HEIGHT = 280;					//!< �q�b�v�h���b�v�G�t�F�N�g�̍���
			constexpr auto HIPDROP_MAX = 24;							//!< �q�b�v�h���b�v�G�t�F�N�g�̑�������
		}
		/** �\���W���[�h�[�� */
		namespace soldier {
			constexpr auto OMEN = "sd_omen_effect";				//!< ���@�����G�t�F�N�g
			constexpr auto OMEN_IMAGE = 160;							//!< ���@�����G�t�F�N�g�̃T�C�Y
			constexpr auto OMEN_NUMS = 14;								//!< ���@�����G�t�F�N�g�̑�������
		}
		/** �r�b�O�h�[�� */
		namespace bigdoll {
			constexpr auto OMEN = "bd_omen_effect";				//!< ���@�����G�t�F�N�g
			constexpr auto OMEN_IMAGE = 280;							//!< ���@�����G�t�F�N�g�̃T�C�Y
			constexpr auto OMEN_NUMS = 14;								//!< ���@�����G�t�F�N�g�̑�������
		}
	}
	/** �G�t�F�N�g�̃X�[�p�[�N���X */
	class EffectBase {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param game			�Q�[���N���X�̎Q��
		 * @param gh				�摜�L�[
		 * @param spawnpos	�������W
		 * @param maxFrame	�����t���[����
		 * @param direction	���]�t���O
		 */
		EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~EffectBase() = default;
		/**
		 * @brief	����������
		 */
		virtual void Init();
		/**
		 * @brief	�X�V����
		 */
		virtual void Process();
		/**
		 * @brief	�`�揈��
		 */
		virtual void Draw();
		// �_���[�W����̐ݒ�(����1:�����@����2:�c���@����3:�_���[�W���肪��������܂ł̗P�\����)
		/**
		 * @brief					�_���[�W����̐ݒ�
		 * @param width		����
		 * @param height	�c��
		 * @param dinter	�_���[�W���肪��������܂ł̗P�\�t���[��
		 */
		virtual void SetDamageEffect(int width, int height, int dinter = 0);
		/**
		 * @brief					�_���[�W����̐ݒ�
		 * @param width1	����(min)
		 * @param width2	����(max)
		 * @param height1	�c��(min)
		 * @param height2	�c��(max)
		 * @param dinter	�_���[�W���肪��������܂ł̗P�\�t���[��
		 * @param max			�_���[�W����̏I���t���[��
		 */
		virtual void SetDamageEffect(int width1, int width2, int height1, int height2, int dinter = 0, int max = 0);
		/**
		 * @brief			���[�v�̐ݒ�
		 * @param max	���[�v��
		 */
		void SetLoop(int max);
		/**
		 * @brief		�����t���O�̎擾
		 * @return	�����t���O��Ԃ�
		 */
		inline bool IsDel() { return _delete; }
	protected:
		Game& _game;						//!< �Q�[���N���X�Q��
		Vector2 _position;			//!< ���W
		AABB _collision;				//!< �����蔻��
		int _count;							//!< �J�E���^
		int _alive;							//!< ��������
		int _allNum;						//!< ���`�文��
		int _interval;					//!< ���[�V��������������܂ł̗P�\����
		int _dInter;						//!< �_���[�W���肪��������܂ł̗P�\����
		int _dMax;							//!< �_���[�W����̏I������
		int _loop;							//!< ���[�v�񐔁i�f�t�H���g���ƂȂ��j
		bool _direction;				//!< �������̌���
		bool _isDamage;					//!< �_���[�W����͂��邩�H
		bool _delete;						//!< �������邩�̔���
		std::string _graphKey;	//!< �O���t�B�b�N�n���h���̃L�[
		/**
		 * @brief			�O���t�B�b�N�n���h���̌���
		 * @param gh	�O���t�B�b�N�n���h���i�[�p�̃A�h���X
		 */
		void GraphResearch(int* gh);
		/**
		 * @brief	���@�Ƃ̐ڐG���肨��у_���[�W����̌Ăяo������
		 */
		void Damage();
		/**
		 * @brief		���@�����E�ǂ��瑤�ɋ���̂��̔���
		 * @return	�E���ɋ���ꍇ��true��Ԃ�
		 *					�����ɋ���ꍇ��false��Ԃ�
		 */
		bool IsPlayerPosition();
		/**
		 * @brief		�_���[�W�������s�����̔���
		 * @return	�_���[�W���肪����ꍇ��true��Ԃ�
		 *					����ȊO�̏ꍇ��false��Ԃ�
		 */
		bool IsDamage();
		/**
		 * @brief		�A�j���[�V�����ԍ��̎擾
		 * @return	�Z�o�����A�j���[�V�����ԍ���Ԃ�
		 */
		int GraphNumber();
	};
}

