#pragma once
#include <unordered_map>
#include "Vector2.h"

namespace inr {

	namespace stage {
		// �X�e�[�W�ǂݍ��ݗp�̃L�[
		constexpr auto CHANGE_NULL = "change_null";	// �ʏ��Ԃ̃L�[�؂�ւ�

		constexpr auto STAGE_0 = "stage0";	// �`���[�g���A��
		constexpr auto STAGE_1 = "stage1";		// �X�e�[�W1(�p��)
		constexpr auto STAGE_2 = "stage2";	// �X�e�[�W2(�X��-����)
		constexpr auto STAGE_2_1 = "stage2-1";	// �X�e�[�W2(�X��-�㕔)
		constexpr auto STAGE_2_2 = "stage2-2";	// �X�e�[�W2(�X��-����)
		constexpr auto STAGE_3 = "boss_stage";	// �X�e�[�W3(�{�X��)
	}

	class Game;
	// �J�ڌ�̃L�[�ƍ��W
	class Transition {
	public:
		Transition(std::string stageKey, Vector2 position);
		~Transition() = default;

		inline std::string NextKey() { return _nextKey; }
		inline Vector2 NextPosition() { return _position; }
	private:
		std::string _nextKey;	// �J�ڐ�̃L�[
		Vector2 _position;	// �J�ڌ�̍��W(���@)
	};

	// �X�e�[�W�J�ڎ��s�p
	class StageTransition {
	public:
		StageTransition(Game& game);
		~StageTransition();

		bool Init();
		bool IsStageChange(const int no);	// �X�e�[�W�J�ڗp�`�b�v�ɐڐG�������H


		std::string StageChange(Vector2& _pos);
	private:
		Game& _game;	// �Q�[��
		int _number;	// �ڐG�����`�b�v

		void Clear();	// 
		std::unordered_map<int, Transition> _transitions;
	};
}

