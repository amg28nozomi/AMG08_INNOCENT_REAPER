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
			constexpr auto CROW_RASH = "crow_rash";	// �A��
			constexpr auto CROW_BLINK = "crow_brink";	// ���ォ��̋��P
			constexpr auto CROW_ROAR = "crow_roar";
			constexpr auto CROW_WINCE = "crow_wince";
			// �摜�T�C�Y
			constexpr auto CROW_SIZE = 380;	// �摜�T�C�Y

		}
	}

	class CrowDoll : public EnemyBase {
	public:
		CrowDoll(Game& game);

		void Init() override;
		void Process() override;
		void Draw() override;

		void WakeUp();	// �����J

		void SetParameter(ObjectValue objValue) override;
	private:
		enum class CrowState {
			// ���S�A�ҋ@�A�f�o�t�A���K(�s���s�\)�A�A���a��A���ォ��̋��P�A���݁A�񊈓����
			DEATH, IDOL, DEBUF, ROAR, RASH, BLINK, WINCE, SLEEP
		};
		CrowState _cState;	// �N���E�h�[���̏��
		Vector2 _target;	// �U���ڕW���W
		int _life;	// �̗�
		int _atkInterval;	// �U���̗P�\����

		void ModeChange(CrowState nextState, std::string key);
		void GetTarget();	// ���@�̍��W���擾����
		bool SetState();	// �e���Ԃ̊Ǘ�
		bool IsAnger();	// �N�����ԂɂȂ��Ă��邩�H
		bool IsVital();	// ���݂̃A�j���[�V�������A���͒D���邩�H
		bool Damage();	// ���g�̃_���[�W�������s��

		void Debuf();	// �f�o�t����
		void AddSoul();		// ���𐶂ݏo��

		AABB NowCollision(std::string key) override;

		void CollisionHit(const std::string ckey, Collision acollision, bool direction) override;
	};
}

