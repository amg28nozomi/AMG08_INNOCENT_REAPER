/*****************************************************************//**
 * @file   Player.h
 * @brief  ���@�N���X�i�I�u�W�F�N�g�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "Vector2.h"
#include "Collision.h"
#include <string>
#include <queue>
#include <unordered_map>

namespace inr {
	// �����p�L�[
	constexpr auto PKEY_DEATH = "player_death";	//!< ���S�i���[�V�������Ȃ��̂Ń_���[�W���[�V�������p�j
	constexpr auto PKEY_IDOL = "player_idol";		//!< �ҋ@
	constexpr auto PKEY_RUN = "player_run";			//!< �ړ�
	constexpr auto PKEY_DASH = "player_dash";		//!< �_�b�V��
	constexpr auto PKEY_ROB = "player_rob";			//!< ����D��
	constexpr auto PKEY_JUMP = "player_jump";		//!<�@�W�����v
	constexpr auto PKEY_GIVE = "player_give";		//!< ����^����
	constexpr auto PKEY_FALL = "player_fall";		//!< ����
	constexpr auto PKEY_HIT = "player_hit";			//!< �_���[�W
	constexpr auto PKEY_CLIMB = "player_climb";	//!< �ӓo��
	constexpr auto PIMAGE_SIZE = 140;						//!< �v���C���[�̕`��T�C�Y
	constexpr auto PLAYER_DEATH_IMAGE = 400;		//!< ���S�G�t�F�N�g�̃T�C�Y
	// �v���C���[�̌���
	constexpr auto PL_LEFT = true;							//!< ������
	constexpr auto PL_RIGHT = false;						//!< �E����
	constexpr auto SOUL_MAX = 3;								//!< �ۗL�ł��鍰�̏��
	/** ��d�C���N���[�h�h�~ */
	class SoulSkin;
	/** ���@ */
	class Player : public ObjectBase {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param game			�Q�[���N���X�̎Q��
		 */
		Player(Game& game);
		/**
		 * @brief						�f�X�g���N�^
		 */
		~Player() override;
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
		 * @brief						�ړ��x�N�g���̎擾
		 * @return					�ړ��x�N�g����Ԃ�
		 */
		inline Vector2 GetMoveVector() override{ return _moveVector; }
		/**
		 * @brief						����ۗL���Ă��邩�̎擾
		 * @return					�v�f������ꍇ��true��Ԃ�
		 *									��̏ꍇ��false��Ԃ�
		 */
		inline bool HaveSoul() { return !_souls.empty(); }
		/**
		 * @brief						���͓������ɓ��B���Ă��邩
		 * @return					���B���Ă���ꍇ��true��Ԃ�
		 *									���B���Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		inline bool IsSoulMax() { return _souls.size() == SOUL_MAX; };
		/**
		 * @brief						���������̎擾
		 * @return					�v�f����Ԃ�
		 */
		inline int SoulsSize() { return static_cast<int>(_souls.size()); }
		/**
		 * @brief						���͎�t�̒�~
		 */
		inline void InputOff() { _input = false; }
		/**
		 * @brief						�f�o�t����
		 * @return					�f�o�t�����ɐ��������ꍇ��true��Ԃ�
		 */
		bool Debuff();
		/**
		 * @brief						�_���[�W����
		 * @param	mv				�m�b�N�o�b�N����
		 * @return					���S��ԂɑJ�ڂ����ꍇ��true��Ԃ�
		 *									�������Ă���ꍇ��false��Ԃ�
		 */
		bool Damage(bool mv);
		/**
		 * @brief						�m�b�N�o�b�N����
		 * @param	mv				�m�b�N�o�b�N����
		 * @return					�����ɐ��������ꍇ��true��Ԃ�
		 *									���G���Ԓ��̏ꍇ��false��Ԃ�
		 */
		bool KnockBack(bool mv);
		/**
		 * @brief						���Z�b�g����
		 * @return					�����ɐ��������ꍇ��true��Ԃ�
		 */
		bool Reset();
		/**
		 * @brief						�I�u�W�F�N�g���̓o�^
		 * @param objValue	�I�u�W�F�N�g���
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief						���W�̍X�V
		 * @param newdata		����:���W�@�E��:����
		 * @param sKey			�X�e�[�W�L�[
		 */
		void SetParameter(std::pair<Vector2, bool> newdata, std::string sKey);
		/**
		 * @brief						���̓o�^
		 * @param soul			��
		 */
		void SoulCatch(std::shared_ptr<SoulSkin> soul);
		/**
		 * @brief						���̏��n
		 * @return					���n���鍰��Ԃ�
		 */
		std::shared_ptr<SoulSkin> GiveSoul();
		/**
		 * @brief						���̎擾
		 * @return					�����i�[����R���e�i��Ԃ�
		 */
		inline std::queue<std::shared_ptr<SoulSkin>> GetSouls() { return _souls; }
	private:
		/**
		 * @brief						�A�N�V������Ԃ�\���񋓌^
		 *									���S�A�ҋ@�A�ړ��A�_�b�V���A�D���A�^����A�m�b�N�o�b�N�A�c�^�o��
		 */
		enum class ActionState {
			DEATH, IDOL, MOVE, DASH, JUMP, FALL, ROB, GIVE, HIT, GRAB
		};
		/**
		 * @brief						��Ԃ�\���񋓌^
		 *									�����A�_���[�W�A���S
		 */
		enum class State {
			ALLIBE, DAMEGE, DEAD
		};
		ActionState _aState;														//!< �A�N�V�������
		Vector2 _moveVector;														//!< �ړ���(�P�ʃx�N�g��)
		std::queue<std::shared_ptr<SoulSkin>> _souls;		//!< �����i�[����R���e�i
		int _judegFrame;																//!< ����t���[����
		int _aFrame;																		//!< �A�N�V�������s�̂��߂̃t���[��
		int _debuffCount;																//!< �f�o�t�J�E���^
		int _hitCount;																	//!< ����t���[��(�A�N�V����)
		int _dashInterval;															//!< �_�b�V���̍Ďg�p�Ԋu
		int _knockBack;																	//!< �m�b�N�o�b�N�J�E���^
		int _invincible;																//!< ���G����
		double _dashX;																	//!< �_�b�V���̍ő�ړ�����(���W)
		double _moveD;																	//!< �ړ��{��
		double _jumpPower;															//!< �W�����v����
		bool _input;																		//!< ���͏������󂯕t���邩
		bool _grab;																			//!< �͂ݔ���
		bool _sChange;																	//!< �X�e�[�W���ς�������ǂ���
		bool _ivxInterval;															//!< ���_���[�W��̃t���O
		std::string _moveType;													//!< �ړ���
		std::string _landingType;												//!< ���n��
		std::pair<double, double> _ivx;									//!< �Ӄ`�b�v�̍��W
		boxs _collisions;																//!< �����蔻��{�b�N�X�i�[�p�R���e�i
		/**
		 * @brief						��ԑJ��
		 * @param nextState	�J�ڂ�����
		 * @param key				�J�ڌ�̃L�[
		 */
		void ChangeState(Player::ActionState nextState, std::string key);
		/**
		 * @brief						���W��񓙂̍X�V
		 */
		void PositionUpdate();
		/**
		 * @brief						�A�N�V��������
		 * @param key				�{�^���̓��͏��
		 */
		bool Action(int key);
		/**
		 * @brief						�_�b�V���̎n��
		 */
		void InputDash();
		/**
		 * @brief						�W�����v�̎n��
		 */
		void InputJump();
		/**
		 * @brief						�ړ�
		 * @param lever			�A�i���O�X�e�B�b�N�̉����͏��
		 */
		void Move(int lever);
		/**
		 * @brief						�͂�
		 * @param leverUD		�A�i���O�X�e�B�b�N�̏c���͏��
		 */
		void Climb(int leverUD);
		/**
		 * @brief						�_�b�V��
		 */
		void Dash();
		/**
		 * @brief						�W�����v
		 */
		void Jump();
		/**
		 * @brief						�D���A�N�V����
		 */
		void Rob();
		/**
		 * @brief						�^����A�N�V����
		 */
		void Give();
		/**
		 * @brief						���̐؂�ւ�����
		 */
		void ChangeSoul();
		/**
		 * @brief						���S����
		 * @return					���S���Ă���ꍇ��true��Ԃ�
		 *									�������Ă���ꍇ��false��Ԃ�
		 */
		bool Dead();
		/**
		 * @brief						���S����
		 */
		void Death();
		/**
		 * @brief						�`�b�v�ɗ����Ă��邩�̔���
		 * @return					�ڐG���Ă���ꍇ��true��Ԃ�
		 *									�ڐG���Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsStandChip() override;
		/**
		 * @brief						���̃_���[�W����
		 */
		void DamageThorm();
		/**
		 * @brief						�͂ݏ�Ԃ��̔���
		 */
		void IsGrab();
		/**
		 * @brief						�͂ݏ���
		 */
		void Grab();
		/**
		 * @brief						��ԑJ�ڂ���т��̑�����
		 */
		void StateUpdate();
		/**
		 * @brief						���݂̓����蔻��̎擾
		 * @param key				���݂̏�ԃL�[
		 * @return					�_�b�V����Ԃ̏ꍇ�̓_�b�V�����̓����蔻���Ԃ�
		 *									����ȊO�̏ꍇ�͒ʏ�̓����蔻���Ԃ�
		 */
		AABB NowCollision(std::string key) override;
		/**
		 * @brief						���͎�t�̍ĊJ
		 */
		void InputOn();
#ifdef _DEBUG
		/**
		 * @brief						�f�o�b�O���̕`��(Debug����)
		 */
		void DebugInfo();
#endif
	};
}
