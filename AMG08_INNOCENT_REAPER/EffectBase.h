/*****************************************************************//**
 * \file   EffectBase.h
 * \brief  �G�t�F�N�g�̃X�[�p�[�N���X
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>

namespace inr {
	namespace effect {
		// �G�t�F�N�g�̃L�[��
		// ���@�G�t�F�N�g
		constexpr auto JUMP = "jump_effect";			// �W�����v
		constexpr auto DASH = "dash_effect";			// �_�b�V��
		constexpr auto ROB = "rob_effect";				// �D��
		constexpr auto HIT = "hit_effect";				// �_���[�W
		constexpr auto DEAD = "dead_effect";			// ���S
		constexpr auto GIVE = "give_effect";			// �^����
		constexpr auto DEBUFF = "playe_debuff_effect";	// �f�o�t
		// �W�����v�G�t�F�N�g�̉摜�T�C�Y
		constexpr auto JUMP_IMAGE_W = 150;
		constexpr auto JUMP_IMAGE_H = 50;
		// �D���G�t�F�N�g�̉摜�T�C�Y
		constexpr auto ROB_IMAGE = 140;
		// �_���[�W�G�t�F�N�g�̉摜�T�C�Y
		constexpr auto HIT_IMAGE_W = 840;
		constexpr auto HIT_IMAGE_H = 430;
		// �^����G�t�F�N�g�̉摜�T�C�Y
		constexpr auto GIVE_IMAGE = 200;
		// �f�o�t�G�t�F�N�g�̉摜�T�C�Y����ё�������
		constexpr auto DEBUF_IMAGE = 140;
		constexpr auto DEBUF_MAX = 15;
		// �G�G�t�F�N�g
		namespace enemy {
			constexpr auto S_HIT = "soul_hit_effect";	// ���S
			constexpr auto SHIT_IMAGE = 400;			// ���S�G�t�F�N�g�̉摜�T�C�Y
			constexpr auto HITDROP = "hipdrop_effect";	// �q�b�v�h���b�v(�Ռ��g)
			constexpr auto HIPDROP_WIDTH = 560;			// �q�b�v�h���b�v�̉���
			constexpr auto HIPDROP_HEIGHT = 280;		// �q�b�v�h���b�v�̗���
			constexpr auto HIPDROP_MAX = 24;			// �q�b�v�h���b�v�̑�������
		}
		// �\���W���[�h�[��
		namespace soldier {
			constexpr auto OMEN = "sd_omen_effect";		// ���@�����G�t�F�N�g
			constexpr auto OMEN_NUMS = 14;				// ��������
			constexpr auto OMEN_IMAGE = 160;			// �摜�T�C�Y
		}
		// �r�b�O�h�[��
		namespace bigdoll {
			constexpr auto OMEN = "bd_omen_effect";		// ���@�����G�t�F�N�g
			constexpr auto OMEN_NUMS = 14;				// ��������
			constexpr auto OMEN_IMAGE = 280;			// �摜�T�C�Y
		}
	}
	// �G�t�F�N�g�̊��N���X
	class EffectBase {
	public:
		// �R���X�g���N�^(����1:Game�N���X�̎Q�Ɓ@����2:�摜�L�[�@����3:�����n�_�@����4:�����t���[�����@����5:���]�t���O)
		EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);
		// �f�X�g���N�^
		~EffectBase() = default;
		// ������
		virtual void Init();
		// �X�V
		virtual void Process();
		// �`��
		virtual void Draw();
		// �_���[�W����̐ݒ�(����1:�����@����2:�c���@����3:�_���[�W���肪��������܂ł̗P�\����)
		virtual void SetDamageEffect(int width, int height, int dinter = 0);
		// �_���[�W����̐ݒ�(����1:����(min)�@����2:����(max)�@����3:�c��(min)�@����4:�c��(max)�@����5:�_���[�W���肪��������܂ł̗P�\���ԁ@����6:�_���[�W����̏I������)
		virtual void SetDamageEffect(int width1, int width2, int height1, int height2, int dinter = 0, int max = 0);
		// ���[�v�ݒ�(����1:���[�v��)
		void SetLoop(int max);
		// ���̃G�t�F�N�g���폜���邩�H
		inline bool IsDel() { return _delete; }
	protected:
		Game& _game;			// �Q�[���N���X�Q��
		Vector2 _position;		// ���W
		AABB _collision;		// �����蔻��
		int _count;				// �J�E���^
		int _alive;				// ��������
		int _allNum;			// ���`�文��
		int _interval;			// ���[�V��������������܂ł̗P�\����
		int _dInter;			// �_���[�W���肪��������܂ł̗P�\����
		int _dMax;				// �_���[�W����̏I������
		int _loop;				// ���[�v�񐔁i�f�t�H���g���ƂȂ��j
		bool _direction;		// �������̌���
		bool _isDamage;			// �_���[�W����͂��邩�H
		bool _delete;			// �������邩�̔���
		std::string _graphKey;	// �O���t�B�b�N�n���h���̃L�[
		// �O���t�B�b�N�n���h���̌���
		void GraphResearch(int* gh);
		// ���@�Ƃ̐ڐG���肨��у_���[�W����̌Ăяo������
		void Damage();
		// ���@�����E�ǂ��瑤�ɋ���̂��̔���(true:�E���ɋ���@false:�����ɋ���j
		bool IsPlayerPosition();
		// �_���[�W�������s�����̔���
		bool IsDamage();
		// �A�j���[�V�����ԍ��̎Z�o
		int GraphNumber();
	};
}

