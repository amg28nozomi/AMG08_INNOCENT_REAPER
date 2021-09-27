#pragma once
#include "EnemyBase.h"

namespace inr {

	namespace enemy {
		namespace crowdoll {
			// �e�탂�[�V��������уL�[
			constexpr auto CROW_ARM = "crow_arm";	// �r
			constexpr auto CROW_IDOL = "crow_stand";	// �ҋ@
			constexpr auto CROW_DEBUF = "crow_debuf";	// �f�o�t���[�V����
			constexpr auto CROW_GROWARM = "crow_growarm";	// �L�т��
			constexpr auto CROW_RUSH = "crow_rash";	// �A��
			constexpr auto CROW_BLINK = "crow_brink";	// ���ォ��̋��P
			constexpr auto CROW_ROAR = "crow_roar";
			constexpr auto CROW_WINCE = "crow_wince";
			constexpr auto CROW_DOWN = "crow_down";	// �|���
			// �摜�T�C�Y
			constexpr auto CROW_SIZE = 500;	// �摜�T�C�Y


			constexpr auto SE_VOICE = "crow_voice";	// ����
			constexpr auto SE_RUSH = "crow_rush";	// �A��
		}
	}

	class CrowDoll : public EnemyBase {
	public:
		CrowDoll(Game& game);

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(ObjectValue objValue) override;
	private:
		enum class CrowState {
			// ���S�A�ҋ@�A�f�o�t�A���K(�s���s�\)�A�A���a��A���ォ��̋��P�A�L�т�܁A���݁A�񊈓����
			DEATH, IDOL, DEBUF, ROAR, RUSH, BLINK, GROWARM, WINCE, SLEEP
		};
		CrowState _cState;	// �N���E�h�[���̏��
		Vector2 _target;	// �U���ڕW���W
		Vector2 _actionEnd;	// �A�N�V�����I��
		int _life;	// �̗�
		int _atkInterval;	// �U���̗P�\����
		int _pattern;	// �s���p�^�[��

		int _actionCount;	// ���b�V���J�E���g

		bool _setup;	// �����͊����������H
		bool _arm;	// �r�͐����������H
		int _muteki;	// ���G����


		void ModeChange(CrowState nextState, std::string key);
		void GetTarget();	// ���@�̍��W���擾����
		bool SetState();	// �e���Ԃ̊Ǘ�
		int IsAnger();	// �N�����ԂɂȂ��Ă��邩�H
		bool IsVital();	// ���݂̃A�j���[�V�������A���͒D���邩�H
		// bool Damage();	// ���g�̃_���[�W�������s��
		void Warp();	// ������W�܂Ń��[�v����
		void WakeUp();	// �����J

		void ChangeDirection();	// �����̕ύX
		bool IsActive();	// ������Ԃɂ��邩�H
		bool IsBattle();

		void Move() override;	// �ړ�
		bool Floating();	// ���V����
		void Debuf();	// �f�o�t����
		void Rash();	// �A���U��
		void Blink();	// �����U��
		void AddSoul();		// ���𐶂ݏo��
		void Attack();	// �U������

		bool IsGravity();	// �d�͏������s�����H
		bool IsAttackEnd();	// �����I��

		bool IsPlayerPos(double px);	// ���@�͍��E�ǂ���ɂ���̂��H

		AABB NowCollision(std::string key) override;

		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

