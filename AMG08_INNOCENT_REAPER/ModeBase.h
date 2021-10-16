/*****************************************************************//**
 * @file   ModeBase.h
 * @brief  ���[�h�̃X�[�p�[�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>

namespace inr {
	/** ���[�h�̃R���p�C�����萔 */
	namespace mode {
		// �e�탂�[�h�̃L�[���
		constexpr auto TITLE = "mode_title";	// �^�C�g��
		constexpr auto MAIN = "mode_main";		// �{��
		constexpr auto FIN = "mode_end";		// �I��
	}
	/** ��d�C���N���[�h�h�~ */
	class Game;
	class ObjectServer;
	class ModeServer;
	/** ���[�h�x�[�X */
	class ModeBase {
	protected:
		Game& _game;						//!< �Q�[���N���X�Q��
		static int _modeFrame;	//!< �o�߃t���[��
		bool _update;						//!< �������X�V���邩�ǂ���
		bool _resetFlg;					//!< ���[�h�؂�ւ����ɏ��������邩�ǂ���
		std::string _bgmKey;		//!< bgm�̃L�[
		/**
		 * @brief		BGM�̊Ǘ�
		 * @return	BGM���~�����ꍇ��true��Ԃ�
		 *					BGM���Đ������ꍇ��false��Ԃ�
		 */
		bool BgmManage();
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		ModeBase(Game& game);
		/**
		 * @brief		�f�X�g���N�^
		 */
		~ModeBase();
		/**
		 * @brief		����������
		 */
		virtual void Init();
		/**
		 * @brief		�X�V����
		 */
		virtual void Process();	
		/**
		 * @brief		�`�揈��
		 */
		virtual void Draw();
		/**
		 * @brief		�X�e�[�W�L�[�̐؂�ւ�
		 * @param		���̃X�e�[�W�L�[
		 */
		virtual void ChangeKey(const std::string nextStage);
		/**
		 * @brief		�������t���O�̃I��
		 */
		inline void Reset() { _resetFlg = true; }
		/**
		 * @brief		�o�߃t���[���̏�����
		 */
		inline void TimeClear() { _modeFrame = 0; };
		/**
		 * @brief		�o�߃t���[�����擾
		 * @return	�o�߃t���[����Ԃ�
		 */
		inline int GetFrame() { return _modeFrame; }
		/**
		 * @brief		�L�[�̎擾
		 * @return	""��Ԃ�
		 */
		virtual inline std::string GetKey() { return ""; }
	};
}