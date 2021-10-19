/*****************************************************************//**
 * @file   ModeMain.h
 * @brief  �Q�[���{�҂��Ǘ����郂�[�h���C���N���X�i���[�h�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "BackGround.h"
#include "Vector2.h"
#include "ImageServer.h"
#include <memory>

namespace inr {
	/** ��d�C���N���[�h�h�~ */
	class Pause;
	class EffectServer;
	class StageUi;
	class ForeGround;
	class ItemServer;
	class TutorialServer;
	/** �Q�[���{�� */
	class ModeMain : public ModeBase {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param game			�Q�[���N���X�̎Q��
		 */
		ModeMain(Game& game);
		/**
		 * @brief						�f�X�g���N�^
		 */
		~ModeMain();
		/**
		 * @brief						����������
		 */
		void Init() override;
		/**
		 * @brief						�X�V����
		 */
		void Process() override;
		/**
		 * @brief						�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief						�X�e�[�W�L�[�̐؂�ւ�
		 * @param nextStage	���̃X�e�[�W�L�[
		 */
		void ChangeKey(const std::string nextStage) override;
		/**
		 * @brief						�؂�ւ��p�L�[���󂩂̔���
		 * @return					�؂�ւ���Ă��Ȃ��ꍇ��true��Ԃ�
		 *									�؂�ւ���Ă���ꍇ��false��Ԃ�
		 */
		bool IsKeyNull();
		/**
		 * @brief						�Q�[���I�[�o�[
		 * @return					�Q�[���I�[�o�[�����ɐ��������ꍇ��true��Ԃ�
		 *									���s�����ꍇ��false��Ԃ�
		 */
		bool GameOver();
		/**
		 * @brief						�{�X�X�e�[�W�ւ̔����������
		 */
		bool OpenBossStage();
		/**
		 * @brief						�X�e�[�W�L�[�̎擾
		 * @return					�X�e�[�W�L�[��Ԃ�
		 */
		inline std::string StageKey() { return _stageKey; }
		/**
		 * @brief						�{�X���̊J�t���O�̎擾
		 * @return					�{�X���̊K�փt���O��Ԃ�
		 */
		inline bool BossOpen() { return _bossOpen; }
		/**
		 * @brief						�{�X��t���O�̎擾
		 * @return					�{�X��t���O��Ԃ�
		 */
		inline bool BossFlag() { return _bossBattle; }
		/**
		 * @brief						�X�e�[�W���̃��Z�b�g
		 * @return					���Z�b�g�ɐ��������ꍇ��true��Ԃ�
		 *									���Z�b�g�Ɏ��s�����ꍇ��false��Ԃ�
		 */
		bool StageReset();
		/**
		 * @brief						�{�X��J�n
		 * @return					�{�X����J�n�����ꍇ��true��Ԃ�
		 *									���s�����ꍇ��false��Ԃ�
		 */
		bool BossBattle();
		/**
		 * @brief						�{�X����I�����邩		
		 * @return					�{�X����I�������ꍇ��true��Ԃ�
		 *									���s�����ꍇ��false��Ԃ�
		 */
		bool BossEnd();
		/**
		 * @brief						�G�t�F�N�g�T�[�o�̎擾
		 * @return					�G�t�F�N�g�T�[�o��Ԃ�
		 */
		std::shared_ptr<EffectServer> GetEffectServer() { return _eServer; }
		/**
		 * @brief						���b�Z�[�W�T�[�o�̎擾
		 * @return					���b�Z�[�W�T�[�o��Ԃ�
		 */
		std::unique_ptr<ImageServer>& GetItemImages() { return _messageServer; }
		/**
		 * @brief						�A�C�e���T�[�o�̎擾
		 * @return					�A�C�e���T�[�o��Ԃ�
		 */
		std::unique_ptr<ItemServer>& GetItemServer() { return _itemServer; }
		/**
		 * @brief						�`���[�g���A���T�[�o�̎擾
		 * @return					�`���[�g���A���T�[�o��Ԃ�
		 */
		std::unique_ptr<TutorialServer>& GetTutorialServer() { return _tutorialServer; }
	private:
		std::string _stageKey;														//!< �X�e�[�W�����p
		std::string _changeKey;														//!< �X�e�[�W�؂�ւ��悤
		std::unique_ptr<BackGround> _bg;									//!< �w�i
		std::unique_ptr<StageUi> _stageUi;								//!< �X�e�[�WUI
		std::unique_ptr<Pause> _pause;										//!< �|�[�Y
		std::shared_ptr<EffectServer> _eServer;						//!< �G�t�F�N�g�T�[�o
		std::unique_ptr<ImageServer> _messageServer;			//!< ���b�Z�[�W�T�[�o
		std::unique_ptr<ForeGround> _fg;									//!< �O�i
		std::unique_ptr<ItemServer> _itemServer;					//!< �A�C�e���T�[�o
		std::unique_ptr<TutorialServer> _tutorialServer;	//!< �`���[�g���A���T�[�o
		int _endCount;																		//!< �I������܂ł̑҂�����
		bool _isReset;																		//!< ���݃X�e�[�W�̏������t���O
		bool _bossOpen;																		//!< �{�X���̊J�t���O
		bool _bossBattle;																	//!< �{�X��t���O
		bool _isEnding;																		//!< �G���f�B���O�ڍs�t���O
		/**
		 * @brief						�X�e�[�W�̐؂�ւ�����
		 * @return					�����ɐ��������ꍇ��true��Ԃ�
		 *									���s�����ꍇ��false��Ԃ�
		 */
		bool IsStageChange();
		/**
		 * @brief						BGM�̊Ǘ�
		 * @param nextStage	���̃X�e�[�W
		 * @return					BGM�̍Đ��ɐ��������ꍇ��true��Ԃ�
		 *									BGM�������ꍇ��false��Ԃ�
		 */
		bool BgmManage(std::string nextStage);
		/**
		 * @brief						�G���f�B���O�ɑJ�ڂ��邩�̔���
		 * @return					�J�ڏ������I���̏ꍇ��true��Ԃ�
		 *									�J�ڂ��Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsEnding();
		/**
		 * @brief						BGM�L�[�̐؂�ւ�
		 * @param	key				���̃X�e�[�W
		 * @return					�q�b�g�����ꍇ�̓L�[��Ԃ�
		 *									�q�b�g���Ȃ������ꍇ��""��Ԃ�
		 */
		std::string BgmKey(std::string key);
	};
}
