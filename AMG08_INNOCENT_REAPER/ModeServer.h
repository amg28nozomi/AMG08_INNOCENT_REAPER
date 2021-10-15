#pragma once
#include <unordered_map>
#include <memory>
#include "ModeBase.h"
#include "Game.h"
#include "FadeBlack.h"

namespace inr {

	class Game;
	class ModeMain;

	class ModeServer {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		ModeServer(Game& game);
		/**
		 * @brief	�f�X�g���N�^
		 */
		~ModeServer();

		void Init();
		void Process();
		void Draw();

		void ModeChange(std::string nextMode, int interval = 0);
		void GameEnd();	// �I���������Ăяo��
		// std::unique_ptr<ModeBase>& GetMode();	// �w�肵�����[�h�̎Q�Ƃ��擾
		std::shared_ptr<ModeMain> GetModeMain();

		inline void FadeOut() { _fadeBlack->FlagChange(image::FADE_OUT, 60); }
		inline bool PalChange() { return _fadeBlack->PalChange(); }
		inline bool IsFadeEnd() { return _fadeBlack->IsEnd(); }
		bool IsFadeChange();
	private:
		using ModeMap = std::unordered_map<std::string, std::shared_ptr<ModeBase>>;

		Game& _game;
		ModeMap _modes;		// ���[�h�i�[�p
		std::unique_ptr<FadeBlack> _fadeBlack;
		std::string _modeKey;	// �����p�L�[���
		std::string _ChangeKey;	// �����L�[�i�[�p

		void ClearModeLists();	// ���[�h�J��
		bool ModeInit();	// ���[�h�̏�����

		void IsModeChange();	// ���[�h�؂�ւ�
	};

}
