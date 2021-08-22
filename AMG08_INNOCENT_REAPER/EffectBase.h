#pragma once
#include "Game.h"
#include "Vector2.h"
#include <string>

namespace inr {

	namespace effect {
		constexpr auto NO_COUNT = 0;
		constexpr auto NO_MAXFRAME = 0;

		// �L�[��
		constexpr auto JUMP = "jump_effect";	// �W�����v�G�t�F�N�g�̃L�[

		// �摜�T�C�Y
		constexpr auto JUMP_IMAGE = 50;
	}

	class Game;

	// �G�t�F�N�g
	class EffectBase {
	public:
		EffectBase(Game& game, const Vector2 spawnpos);
		~EffectBase() = default;

		virtual void Init();
		virtual void Process();
		virtual void Draw();	// 

	private:
		Game& _game;	// �Q�[���Q��

		Vector2 _position;	// ���W
		std::pair<int, int> _count;	// ����:��������(�t���[��)�A�����\�t���[��
		std::string _graphKey;	// �O���t�B�b�N�n���h���̃L�[

		void GraphResearch(int* gh);	// ResourceServer����O���t�B�b�N�n���h���擾
		int GetSize(const std::string& key);	// �A�j���[�V�����̍Đ��t���[��
		void AnimationCount();	// �A�j���[�V�����J�E���^�̑���
		int AnimationInterval();	// �A�j���[�V�����̕`��Ԋu
		int AnimationNumber();	// ���݂̕`��ԍ����擾
		bool IsAnimationMax();	// �A�j���[�V�����͕`�悵�I�������H
		bool AnimationCountMax();
	};
}

