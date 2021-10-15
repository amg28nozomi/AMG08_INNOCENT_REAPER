#pragma once
#include <unordered_map>
#include "Vector2.h"

namespace inr {

	namespace stage {
		// �X�e�[�W�ǂݍ��ݗp�̃L�[
		constexpr auto CHANGE_NULL = "change_null";	// �ʏ��Ԃ̃L�[�؂�ւ�

		constexpr auto STAGE_0 = "stage0";	// �ŏ�
		constexpr auto STAGE_T = "stage0-2";	// �`���[�g���A��
		constexpr auto STAGE_1 = "stage1";		// �X�e�[�W1(�p��)
		constexpr auto STAGE_2 = "stage2";	// �X�e�[�W2(�X��-����)
		constexpr auto STAGE_2_1 = "stage2-1";	// �X�e�[�W2(�X��-�㕔)
		constexpr auto STAGE_2_2 = "stage2-2";	// �X�e�[�W2(�X��-����)
		constexpr auto STAGE_3 = "stageb";	// �X�e�[�W3(�{�X��)
	}

	class Game;
	// �J�ڌ�̃L�[�ƍ��W
	class Transition {
	public:
		/**
		 * @brief	�R���X�g���N�^
		 * @param	�J�ڃX�e�[�W�L�[
		 * @param	�J�ڌ�̍��W
		 * @param	�J�ڌ�̌���
		 */
		Transition(std::string stageKey, Vector2 position, bool direction);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~Transition() = default;

		inline std::string NextKey() { return _nextKey; }
		inline Vector2 NextPosition() { return _position; }
		inline bool NextDirection() { return _direction; }
	private:
		std::string _nextKey;	// �J�ڐ�̃L�[
		Vector2 _position;	// �J�ڌ�̍��W(���@)
		bool _direction;
	};

	// �X�e�[�W�J�ڎ��s�p
	class StageTransition {
	public:
		/**
		 * @brief	�R���X�g���N�^
		 * @param	�Q�[���N���X�̎Q��
		 */
		StageTransition(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~StageTransition();

		bool Init();
		bool IsHit(const int no);	// �Փ˂������H
		bool IsStageChange();	// �J�ڏ������s�����H


		std::pair<Vector2, bool> SetPosition();
	private:
		Game& _game;	// �Q�[��
		int _number;	// �ڐG�����`�b�v

		void Clear();	// 
		std::unordered_map<int, Transition> _transitions;
	};
}

