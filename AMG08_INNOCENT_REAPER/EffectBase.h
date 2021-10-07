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
			constexpr auto OMEN = "sd_omen_effect";
			constexpr auto OMEN_NUMS = 14;
			constexpr auto OMEN_IMAGE = 160;
		}

		namespace bigdoll {
			constexpr auto OMEN = "bd_omen_effect";
			constexpr auto OMEN_NUMS = 14;
			constexpr auto OMEN_IMAGE = 280;
		}
	}

	class Game;

	// �G�t�F�N�g
	class EffectBase {
	public:
		EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);	// �������W����сA�����t���[�����A�������A��摜������̑҂�����
		~EffectBase() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();	// �`��

		virtual void SetDamageEffect(int width, int height, int dinter = 0);	// �G�t�F�N�g�Ƀ_���[�W�������������
		virtual void SetDamageEffect(int width1, int width2, int height1, int height2, int dinter = 0, int max = 0);
		void SetLoop(int max);	// ����1 ����J��Ԃ���
		inline bool IsDel() { return _delete; }	// ���̃G�t�F�N�g���폜���邩�H

	protected:

		Game& _game;	// �Q�[���Q��

		Vector2 _position;	// ���W
		AABB _collision;	// �����蔻��
		int _count;	// �J�E���^
		int _alive;	// ��������
		int _allNum;	// ���`�文��
		int _interval;	// ���[�V��������������܂ł̗P�\����]
		int _dInter;	//	�_���[�W���肪��������܂ł̗P�\����
		int _dMax;
		int _loop;	// ���[�v�񐔁i�f�t�H���g���ƂȂ��j
		bool _direction;	// �������̌���
		bool _isDamage;	// �_���[�W����͂��邩�H
		bool _delete;	// �������邩�̔���
		std::string _graphKey;	// �O���t�B�b�N�n���h���̃L�[

		void GraphResearch(int* gh);	// ResourceServer����O���t�B�b�N�n���h���擾
		void Damage();	// �_���[�W�����i���@�ցj
		bool IsPlayerPosition();
		bool IsDamage();
		int GraphNumber();
	};
}

