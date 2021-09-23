#pragma once
#include "Game.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>

namespace inr {

	namespace effect {
		constexpr auto NO_COUNT = 0;
		constexpr auto NO_MAXFRAME = 0;

		// �L�[��
		constexpr auto JUMP = "jump_effect";	// �W�����v�G�t�F�N�g�̃L�[
		constexpr auto DASH = "dash_effect";
		constexpr auto ROB = "rob_effect";
		constexpr auto HIT = "hit_effect";	// ���@�_���[�W��
		constexpr auto DEAD = "dead_effect";	// �_���[�W���G�t�F�N�g
		constexpr auto GIVE = "give_effect";	// �^����G�t�F�N�g

		constexpr auto S_HIT = "soul_hit_effect";

		// �摜�T�C�Y
		constexpr auto JUMP_IMAGE_W = 80;
		constexpr auto JUMP_IMAGE_H = 50;
		constexpr auto ROB_IMAGE = 140;	// �D���G�t�F�N�g�̉摜�T�C�Y
		constexpr auto HIT_IMAGE_W = 840;
		constexpr auto HIT_IMAGE_H = 430;

		constexpr auto SHIT_IMAGE = 400;
		constexpr auto GIVE_IMAGE = 140;
	}

	class Game;

	// �G�t�F�N�g
	class EffectBase {
	public:
		EffectBase(Game& game, const std::string gh, const Vector2 spawnpos, const int maxFrame, const bool direction = false);	// �������W����сA�����t���[�����A�������A��摜������̑҂�����
		~EffectBase() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();	// 

		virtual void SetDamageEffect(int width, int height);	// �G�t�F�N�g�Ƀ_���[�W�������������
		inline bool IsDel() { return _delete; }	// ���̃G�t�F�N�g���폜���邩�H

	protected:

		Game& _game;	// �Q�[���Q��

		Vector2 _position;	// ���W
		AABB _collision;	// �����蔻��
		int _count;	// �J�E���^
		int _alive;	// ��������
		int _allNum;	// ���`�文��
		int _interval;	// ���[�V��������������܂ł̗P�\����]
		bool _direction;	// �������̌���
		bool _isDamage;	// �_���[�W����͂��邩�H
		bool _delete;	// �������邩�̔���
		std::string _graphKey;	// �O���t�B�b�N�n���h���̃L�[

		void GraphResearch(int* gh);	// ResourceServer����O���t�B�b�N�n���h���擾
		void Damage();	// �_���[�W�����i���@�ցj
		bool IsPlayerPosition();
	};
}

