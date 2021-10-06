#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		namespace crowdoll {
			// �e�탂�[�V��������уL�[
			constexpr auto CROW_ARM = "crow_arm";	// �r
			constexpr auto CROW_IDOL = "crow_stand";	// �ҋ@
			constexpr auto CROW_DEBUFF = "crow_debuf";	// �f�o�t���[�V����
			constexpr auto CROW_GROWARM = "crow_growarm";	// �L�т��
			constexpr auto CROW_RUSH = "crow_rash";	// �A��
			constexpr auto CROW_BLINK = "crow_brink";	// ���ォ��̋��P
			constexpr auto CROW_ROAR = "crow_roar";
			constexpr auto CROW_WINCE = "crow_wince";
			constexpr auto CROW_DOWN = "crow_down";	// �|���
			// �摜�T�C�Y
			constexpr auto CROW_SIZE = 500;	// �摜�T�C�Y


			constexpr auto SE_VOICE = "crow_voice";	// ����
			constexpr auto SE_ROAR = "crow_roar_voice";	// ���K�{�C�X
			constexpr auto SE_RUSH = "crow_rush";	// �A��
			constexpr auto SE_GROWARM = "crow_growarm";	// �n���U��
			constexpr auto SE_BLINK_ATTACK = "crow_blink_attack";
			constexpr auto SE_DEBUF = "crow_debuf";	// �f�o�t�{�C�X
			constexpr auto SE_DEATH = "crow_death";	// ���SSE

			// ���[�V������
			namespace motion {
				constexpr auto IDOL = 13;
				constexpr auto RUSH = 35;
				constexpr auto BLINK = 13;
				constexpr auto GROWARM = 10;
				constexpr auto ROAR = 11;
				constexpr auto DEBUF = 22;
				constexpr auto WINCE = 7;
				constexpr auto DOWN = 26;
			}
		}
	}

	namespace effect {
		namespace crow {
			constexpr auto ROAR = "cd_roar_effect";	// ���K
			constexpr auto ARM = "cd_arms_effect";	// �r
			constexpr auto RUSH = "cd_rush_effect";	// �A���؂�
			constexpr auto GROW = "cd_grow_chage_effect"; // �n���h������
			constexpr auto BLINK = "cd_blink_effect";	// �]��
			constexpr auto BLINK_ATTACK = "cd_blink_attack_effect";	// �����U��
			constexpr auto DEBUF = "cd_debuf_effect";	// �f�o�t�G�t�F�N�g
			constexpr auto AURA = "cd_aura_effect"; // �{��G�t�F�N�g(�I�[��)
			constexpr auto DEATH = "cd_death_effect";	// ���S�G�t�F�N�g

			// ���K�G�t�F�N�g�̉摜�T�C�Y
			constexpr auto ROAR_WIDTH = 3840;
			constexpr auto ROAR_HEIGHT = 2160;
			// �r�G�t�F�N�g�̉摜�T�C�Y
			constexpr auto ARM_WIDTH = 750;
			constexpr auto ARM_HEIGHT = 600;
			constexpr auto GROW_IMAGE = 400;
			// �A���؂�G�t�F�N�g�̉摜�T�C�Y
			constexpr auto RUSH_WIDTH = 800;
			constexpr auto RUSH_HEIGHT = 600;
			// ���[�v�G�t�F�N�g�̉摜�T�C�Y
			constexpr auto BLINK_IMAGE = 800;
			// �����U���G�t�F�N�g�̉摜�T�C�Y
			constexpr auto BLINK_ATTACK_W = 560;
			constexpr auto BLINK_ATTACK_H = 800;
			// �����U���G�t�F�N�g�̕�����
			constexpr auto BLINL_ATTACK_MAX = 10;
			// �I�[���G�t�F�N�g�̉摜�T�C�Y
			constexpr auto AURA_IMAGE = 500;
			// �I�[���G�t�F�N�g�̕�����
			constexpr auto AURA_MAX = 30;
			// �f�o�t�G�t�F�N�g�̑��摜����
			constexpr auto DEBUF_MAX = 10;
			// ���S�G�t�F�N�g�̉摜�T�C�Y
			constexpr auto DEATH_IMAGE = 600;
			// ���S�G�t�F�N�g�̕�����
			constexpr auto DEATH_MAX = 14;

		}
	}

	// �{�X
	class CrowDoll : public EnemyBase {
	public:
		CrowDoll(Game& game);

		void Init() override;
		void Process() override;
		void Draw() override;

		// �I�u�W�F�N�g���̓o�^
		void SetParameter(ObjectValue objValue) override;
	private:
		enum class CrowState {
			// ���S�A�ҋ@�A�f�o�t�A���K(�s���s�\)�A�A���a��A���ォ��̋��P�A�L�т�܁A���݁A�񊈓����
			DEATH, IDOL, DEBUF, ROAR, RUSH, BLINK, GROWARM, WINCE, SLEEP
		};
		CrowState _cState;	// �N���E�h�[���̏��
		Vector2 _target;	// �U���ڕW���W
		Vector2 _actionEnd;	// �A�N�V�����I��
		Vector2 _warpPos;	// �]�ڌ�̍��W
		int _life;	// �̗�
		int _atkInterval;	// �U���̗P�\����
		int _pattern;	// �s���p�^�[��
		int _debuffCount;	// �f�o�t�𔭓����邩�H

		int _actionCount;	// ���b�V���J�E���g
		int _debuffChage;	// �f�o�t����

		bool _setup;	// �����͊����������H
		bool _arm;	// �r�͐����������H
		bool _debuf;	// �f�o�t�t���O
		bool _changeState;	// ��Ԃ�؂�ւ��邩�H
		bool _isAnimation;	// ���[�V�����Đ����s����
		bool _isWarp;	// ���[�v���s�����H
		bool _isAnger;	// �{��t���O
		bool _wait;	// �ҋ@�t���O
		int _muteki;	// ���G����

		// ��ԑJ��
		void ModeChange(CrowState nextState, std::string key);
		// ���@�̍��W���擾����
		void GetTarget();
		// �e���Ԃ̊Ǘ�
		bool SetState();
		// �{���Ԃɓ˓����Ă��邩�̔���
		int IsAnger();
		// ����D�����Ԃ��̔���
		bool IsVital();
		// ���[�v�����̋N��
		void WarpOn();
		// ���[�v����
		void Warp();
		// �����J�n
		void WakeUp();
		// �����̕ύX
		void ChangeDirection();	
		// ������Ԃɂ��邩�H
		bool IsActive();
		// �{�X�퓬�t���O���I���ɂ��邩
		bool IsBattle();

		// �ړ�����
		void Move() override;
		// ���V����
		bool Floating();
		// �A���U��
		void Rash();
		// ���𐶂ݏo��
		void AddSoul();
		// �U������
		void Attack();

		// �d�͏������s�����H
		bool IsGravity();
		// �U�����I�����邩�H�i�}�b�v�`�b�v�ƏՓ˂��Ă��邩�H�j
		bool IsAttackEnd();
		// �{���Ԃւ̑J��
		bool AngerOn();

		// ���@�͍��E�ǂ���ɂ���̂��H
		bool IsPlayerPos(double px);
		// ����ł��邩�H
		bool IsDead() override;
		// ���S�����̋N��
		bool DeathOn();
		// ���h�[��(�G)�����S������
		bool DollsEnd();

		// ���݂̓����蔻��̎Z�o
		AABB NowCollision(std::string key) override;
		// �����U�����̋}������
		std::pair<AABB, AABB> BlinkVitalPart(Collision& col, int vital);
		// ���@�A�N�V�����Ƃ̏Փ˔���
		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;

		// ���[�v�G�t�F�N�g�̐���(����1:�����n�_�A����2:�Ǐ]�������s�����H(true:�Ǐ]�����L��, false:�Ǐ]��������)
		bool AddWarpEffect(Vector2 spwan, bool target = false);
		// �A���G�t�F�N�g�̐���
		bool AddRushEffect();
		// �U���G�t�F�N�g�̐���
		bool AddBlinkEffect();
		// ��(�Ռ��g)�G�t�F�N�g�̐���
		bool AddSmokeEffect();
		// �{��G�t�F�N�g�̐���
		bool AddAngerEffect();
		// �f�o�t�G�t�F�N�g�̐���
		bool AddDebufEffect();
		// ���@�͍��E�ǂ���ɂ���̂�
		bool IsPlayerPosition();
		// �U������̎Z�o
		bool AttackBox(bool flag);
	};
}

