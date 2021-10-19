/*****************************************************************//**
 * @file   CrowDoll.h
 * @brief  �{�X�G�l�~�[�̃N���E�h�[���N���X�i�G�l�~�[�x�[�X�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "EnemyBase.h"

namespace inr {
	/** �G�̃R���p�C�����萔 */
	namespace enemy {
		/** �N���E�h�[���̃R���p�C�����萔 */
		namespace crowdoll {
			// �e�탂�[�V��������уL�[
			constexpr auto CROW_ARM = "crow_arm";									//!< �r
			constexpr auto CROW_IDOL = "crow_stand";							//!< �ҋ@
			constexpr auto CROW_DEBUFF = "crow_debuf";						//!< �f�o�t���[�V����
			constexpr auto CROW_GROWARM = "crow_growarm";					//!< �L�т��
			constexpr auto CROW_RUSH = "crow_rash";								//!< �A��
			constexpr auto CROW_BLINK = "crow_brink";							//!< ���ォ��̋��P
			constexpr auto CROW_ROAR = "crow_roar";								//!< ���K
			constexpr auto CROW_WINCE = "crow_wince";							//!< ����
			constexpr auto CROW_DOWN = "crow_down";								//!< �|���
			// �摜�T�C�Y
			constexpr auto CROW_SIZE = 500;												//!< �����摜1��������̉摜�T�C�Y
			// SE�L�[�iSoundServer�j
			constexpr auto SE_VOICE = "crow_voice";								//!< ����(SE)
			constexpr auto SE_ROAR = "crow_roar_voice";						//!< ���K�{�C�X(SE)
			constexpr auto SE_RUSH = "crow_rush";									//!< �A���U��(SE)
			constexpr auto SE_GROWARM = "crow_growarm";						//!< �n���U��(SE)
			constexpr auto SE_BLINK_ATTACK = "crow_blink_attack";	//!< �����U��(SE)
			constexpr auto SE_DEBUF = "crow_debuf";								//!< �f�o�t�{�C�X(SE)
			constexpr auto SE_DEATH = "crow_death";								//!< ���S(SE)
			/** ���[�V���� */
			namespace motion {
				constexpr auto IDOL = 13;														//!< �ҋ@���[�V�����̑�������
				constexpr auto RUSH = 35;														//!< �A���U�����[�V�����̑�������
				constexpr auto BLINK = 13;													//!< �����U�����[�V�����̑�������
				constexpr auto GROWARM = 10;												//!< �n���U�����[�V�����̑�������
				constexpr auto ROAR = 11;														//!< ���K���[�V�����̑�������
				constexpr auto DEBUF = 22;													//!< �f�o�t���[�V�����̑�������
				constexpr auto WINCE = 7;														//!< ���݃��[�V�����̑�������
				constexpr auto DOWN = 26;														//!< �_�E�����[�V�����̑�������
			}
		}
	}
	/** �G�t�F�N�g */
	namespace effect {
		/** �N���E�h�[���G�t�F�N�g */
		namespace crow {
			// �G�t�F�N�g�̉摜�L�[(ResourceServer)
			constexpr auto ROAR = "cd_roar_effect";									//!< ���K�G�t�F�N�g
			constexpr auto ARM = "cd_arms_effect";									//!< �r�G�t�F�N�g
			constexpr auto RUSH = "cd_rush_effect";									//!< �A���؂�G�t�F�N�g
			constexpr auto GROW = "cd_grow_chage_effect";						//!< �n���h�����߃G�t�F�N�g
			constexpr auto BLINK = "cd_blink_effect";								//!< ���[�v�G�t�F�N�g
			constexpr auto BLINK_ATTACK = "cd_blink_attack_effect";	//!< �����U���G�t�F�N�g
			constexpr auto DEBUF = "cd_debuf_effect";								//!< �f�o�t�G�t�F�N�g
			constexpr auto AURA = "cd_aura_effect";									//!< �{��G�t�F�N�g(�I�[��)
			constexpr auto DEATH = "cd_death_effect";								//!< ���S�G�t�F�N�g
			// ���K�G�t�F�N�g
			constexpr auto ROAR_WIDTH = 3840;												//!< ���K�G�t�F�N�g�̕�
			constexpr auto ROAR_HEIGHT = 2160;											//!< ���K�G�t�F�N�g�̍���
			// �r�G�t�F�N�g
			constexpr auto ARM_WIDTH = 750;													//!< �r�G�t�F�N�g�̕�
			constexpr auto ARM_HEIGHT = 600;												//!< �r�G�t�F�N�g�̍���
			// �A���U���G�t�F�N�g
			constexpr auto RUSH_WIDTH = 800;												//!< �A���U���G�t�F�N�g�̕�										
			constexpr auto RUSH_HEIGHT = 600;												//!< �A���U���G�t�F�N�g�̍���
			// ���[�v�G�t�F�N�g
			constexpr auto BLINK_IMAGE = 800;												//!< ���[�v�G�t�F�N�g�̃T�C�Y
			// �����U���G�t�F�N�g
			constexpr auto BLINK_ATTACK_W = 560;										//!< �����U���G�t�F�N�g�̕�
			constexpr auto BLINK_ATTACK_H = 800;										//!< �����U���G�t�F�N�g�̍���
			constexpr auto BLINL_ATTACK_MAX = 10;										//!< �����U���G�t�F�N�g�̑�������
			// �I�[���G�t�F�N�g
			constexpr auto AURA_IMAGE = 500;												//!< �I�[���G�t�F�N�g�̃T�C�Y
			constexpr auto AURA_MAX = 30;														//!< �I�[���G�t�F�N�g�̑�������
			// �f�o�t�G�t�F�N�g��
			constexpr auto DEBUF_MAX = 10;													//!< �f�o�t�G�t�F�N�g�̑�������
			// ���S�G�t�F�N�g
			constexpr auto DEATH_IMAGE = 600;												//!< ���S�G�t�F�N�g�̃T�C�Y
			constexpr auto DEATH_MAX = 14;													//!< ���S�G�t�F�N�g�̑�������
		}
	}
	/** �N���E�h�[��(�{�X�G�l�~�[) */
	class CrowDoll : public EnemyBase {
	public:
		/**
		 * @brief				�R���X�g���N�^
		 * @param game	�Q�[���N���X�̎Q��
		 */
		CrowDoll(Game& game);
		/**
		 * @brief	����������
		 */
		void Init() override;
		/**
		 * @brief	�X�V����
		 */
		void Process() override;
		/**
		 * @brief	�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief						�I�u�W�F�N�g���̓o�^
		 * @param objValue	�I�u�W�F�N�g���
		 */
		void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief						���̃G�l�~�[�̓{�X���ǂ���
		 * @return					true��Ԃ�
		 */
		inline bool IsBoss() override { return true; }
	private:
		/**
		 * @brief �N���E�h�[���̏�Ԃ�\���񋓌^
		 *				���S�A�ҋ@�A�f�o�t�A���K(�s���s�\)�A�A���a��A���ォ��̋��P�A�L�т�܁A���݁A�񊈓����
		 */
		enum class CrowState {
			DEATH, IDOL, DEBUFF, ROAR, RUSH, BLINK, GROWARM, WINCE, SLEEP
		};
		CrowState _crowState;		//!< �N���E�h�[���̏��
		Vector2 _target;				//!< �U���ڕW���W
		Vector2 _actionEnd;			//!< �A�N�V�����I��
		Vector2 _warpPos;				//!< �]�ڌ�̍��W
		int _life;							//!< �̗�
		int _atkInterval;				//!< �U���̗P�\����
		int _debuffCount;				//!< �f�o�t�𔭓����邩�H
		int _invincible;				//!< ���G����
		int _actionCount;				//!< ���b�V���J�E���g
		int _debuffChage;				//!< �f�o�t����
		bool _setup;						//!< �����͊����������H
		bool _arm;							//!< �r�͐����������H
		bool _debuff;						//!< �f�o�t�t���O
		bool _isAnimation;			//!< ���[�V�����Đ����s����
		bool _isWarp;						//!< ���[�v���s�����H
		bool _isAnger;					//!< �{��t���O
		bool _wait;							//!< �ҋ@�t���O
		/**
		 * @brief						��ԑJ��
		 * @param nextState	�J�ڂ�����
		 * @param key				�J�ڂ���摜�L�[
		 */
		void ModeChange(CrowState nextState, std::string key);
		/**
		 * @brief		�U���ڕW���W�̍X�V
		 */
		void GetTarget();
		/**
		 * @brief		�e���Ԃ̊Ǘ�
		 */
		void SetState();
		/**
		 * @brief		���[�v�����̋N��
		 */
		void WarpOn();
		/**
		 * @brief		���[�v����
		 */
		void Warp();
		/**
		 * @brief		�����J�n����
		 */
		void WakeUp();
		/**
		 * @brief		�����t���O�̐؂�ւ�
		 */
		void ChangeDirection();	
		/**
		 * @brief		�{�X�퓬�t���O���I���ɂ��邩�̔���
		 */
		void IsBattle();
		/**
		 * @brief		�ړ�����
		 */
		void Move() override;
		/**
		 * @brief		���V����
		 */
		void Floating();
		/**
		 * @brief		�A���U������
		 */
		void Rash();
		/**
		 * @brief		���̐�������
		 */
		void AddSoul();
		/**
		 * @brief		�U������
		 */
		void Attack();
		/**
		 * @brief		�d�͏������s�����̔���
		 */
		void IsGravity();
		/**
		 * @brief		���͂̃h�[���𔲂��k�ɂ���
		 */
		void DollsEnd();
		/**
		 * @brief							���@�A�N�V�����{�b�N�X�Ƃ̏Փ˔���
		 * @param	ckey				�Ώۂ̌��݂̏��(�L�[)
		 * @param	acollision	�Ώۂ̓����蔻��{�b�N�X
		 * @param	direction		�Ώۂ̌����t���O
		 */
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
		/**
		 * @brief				���[�v�G�t�F�N�g�̐���
		 * @param spwan	�����n�_
		 */
		void AddWarpEffect(Vector2 spwan);
		/**
		 * @brief			�A���G�t�F�N�g�̐���
		 */
		void AddRushEffect();
		/**
		 * @brief			�����U���G�t�F�N�g�̐���
		 */
		void AddBlinkEffect();
		/**
		 * @brief			��(�Ռ��g)�G�t�F�N�g�̐���
		 */
		void AddSmokeEffect();
		/**
		 * @brief			�{��G�t�F�N�g�̐���
		 */
		void AddAngerEffect();
		/**
		 * @brief			�f�o�t�G�t�F�N�g�̐���
		 */
		void AddDebuffEffect();
		/**
		 * @brief			�{���Ԃɓ˓����Ă��邩�̔���
		 * @return		�{���Ԃ̏ꍇ��1��Ԃ�
		 *						�ʏ��Ԃ̏ꍇ��0��Ԃ�
		 */
		int IsAnger();
		/**
		 * @brief			������Ԃ��̔���
		 * @return		������Ԃ̏ꍇ��true��Ԃ�
		 *						�񊈓���Ԃ̏ꍇ��false��Ԃ�
		 */
		bool IsActive();
		/**
		 * @brief			����D�����Ԃ��̔���
		 * @return		�D����ꍇ��true��Ԃ�
		 *						�D���Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsVital();
		/**
		 * @brief			�U���𒆒f���邩�̔���
		 *						�}�b�v�`�b�v�ƏՓ˂����ꍇ�͒��f
		 * @return		�Փ˂��Ă���ꍇ��true��Ԃ�
		 *						�Փ˂��Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsAttackEnd();
		/**
		 * @brief			�{���Ԃւ̑J�ڂ��s�����̔���
		 * @return		�c��̗͂�������؂��Ă���ꍇ��teuw��Ԃ�
		 *						���ɓ{���Ԃ������͏����𖞂����Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool AngerOn();
		/**
		 * @brief			���@�����E�ǂ��瑤�ɂ��邩�̔���
		 * @param px	���@��X���W
		 * @return		�����ɋ���ꍇ��true
		 *						�E���ɋ���ꍇ��false��Ԃ�
		 */
		bool IsPlayerPos(double px);
		/**
		 * @brief			���S��Ԃ��̔���
		 * @return		���S��Ԃ̏ꍇ��true��Ԃ�
		 *						����ȊO�̏ꍇ��false��Ԃ�
		 */
		bool IsDead() override;
		/**
		 * @brief				���S�����̋N��
		 * @return			�N���ɐ��������ꍇ��true��Ԃ�
		 *							���s�܂��͏����𖞂����Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool DeathOn();
		/**
		 * @brief				���@�����E�ǂ���ɂ��邩�̔���
		 * @return �@�@�@�E���ɋ���ꍇ��true��Ԃ�
		 *							�����ɋ���ꍇ��false��Ԃ�
		 */
		bool IsPlayerPosition();
		/**
		 * @brief				�U������̐؂�ւ�
		 * @param flag	�؂�ւ��p�t���O
		 * @return			�؂�ւ��ɐ��������ꍇ��true��Ԃ�
		 *							�؂�ւ��Ɏ��s�����ꍇ��false��Ԃ�
		 */
		bool AttackBox(bool flag);
		/**
		 * @brief				���݂̓����蔻��̎擾
		 * @param key		���݂̏�ԃL�[
		 * @return			���S��Ԃ̏ꍇ�͔����k�̓����蔻���Ԃ�
		 *							����ȊO�̏ꍇ�͒ʏ�̓����蔻���Ԃ�
		 */
		AABB NowCollision(std::string key) override;
		/**
		 * @brief				�����U�����̋}���̎擾
		 * @param col		�C�����铖���蔻��{�b�N�X
		 * @param vital	�}���͈�
		 * @return			���E�̓����蔻���pair�^�ŕԂ�
		 */
		std::pair<AABB, AABB> BlinkVitalPart(Collision& col, int vital);
	};
}

