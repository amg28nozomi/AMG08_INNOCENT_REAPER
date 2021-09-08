#pragma once
#include "ObjectBase.h"
#include "Collision.h"
// #include "Soul.h"

namespace inr {

	namespace enemy {
		// �L�[���
		constexpr auto SOLDIER_EMPTY = "soldier_empty";	// �����k
		constexpr auto BIG_EMPTY = "big_empty";	// �����k(�r�b�O�h�[��)

		namespace red {	// ���i�ԁj
			/* �\���W���[�h�[�� */
			constexpr auto SOLDIER_WAKEUP = "soldier_r_wakeup";	// �N���オ��
			constexpr auto SOLDIER_IDOL = "soldier_r_stand";	// �A�C�h��
			constexpr auto SOLDIER_PATROL = "soldier_r_patrol";	// ����
			constexpr auto SOLDIER_ATTACK = "soldier_r_attack";	// �U��

			/* �r�b�O�h�[�� */
			constexpr auto BIG_WAKEUP = "big_r_wakeup";		// �N���オ��
			constexpr auto BIG_IDOL = "big_r_stand";		// �ҋ@
			constexpr auto BIG_PATROL = "big_r_patrol";		// ����
			constexpr auto BIG_TACKLE = "big_r_tackle";		// �U���i�^�b�N���j
			constexpr auto BIG_HIPDROP = "big_r_hipdrop";	// �U���i�q�b�v�h���b�v�j
		}

		namespace blue { // �̍�
			constexpr auto SOLDIER_WAKEUP = "soldier_b_wakeup";	// �N���オ��
			constexpr auto SOLDIER_IDOL = "soldier_b_stand";	// �A�C�h��
			constexpr auto SOLDIER_PATROL = "soldier_b_patrol";	// ����
			constexpr auto SOLDIER_ESCAPE = "soldier_b_escape";	// ����

			constexpr auto BIG_WAKEUP = "big_b_wakeup";
			constexpr auto BIG_IDOL = "big_b_stand";
			constexpr auto BIG_PATROL = "big_b_patrol";
			constexpr auto BIG_ESCAPE = "big_b_escape";
		}

		constexpr auto ESCAPE_MAX = 1000;
		constexpr auto ESCAPE_VECTOR = 120;	// 1�b��(60frame)�ňړ����鋗��S

		constexpr auto ATTACK_MAX = 1000;
		constexpr auto ATTACK_VECTOR = 120;

		constexpr auto MOVE_LEFT = true;
		constexpr auto MOVE_RIGHT = false;
	}

	class SoulSkin;

	class EnemyBase : public ObjectBase {
	protected:
		// �\���W���[�h�[���A�r�b�O�h�[���A�N���E�h�[��
		enum class EnemyType {
			SOLDIER_DOLL, BIG_DOLL, CROW_DOLL
		};
		// �����k�A����A�U���A�����A�N���オ��
		enum class ActionState {
			EMPTY, IDOL, PATROL, ATTACK, ESCAPE, WAKEUP
		};

		// Soul _soul;
		EnemyType _eType;
		ActionState _aState;

		std::shared_ptr<SoulSkin> _soul;

		AABB _searchBox; // ���G�p�����蔻��
		boxs _collisions;	// �e�퓖���蔻��i�[�p
		Vector2 _moveVector;	// �ړ��x�N�g��
		int _aFrame;	// �A�N�V�������s�̂��߂̃t���[��
		int _sounds;

		int _aInterval;	// �A�N�V�����C���^�[�o��
		int _stay;	// �ҋ@�J�E���^

		double _actionX;	// �A�N�V�������s����x���W
		double _patrolX;
		bool _isSearch;	// �v���C���[��
		bool _drawStop;	// �`��X�V��~

		void AnimationCount() override;

		bool SearchPlayer(); // ���G����(���@�̍��G

		virtual void Move() = 0; // �ړ�����(�h����Œ�`)
		virtual void Action();
		// virtual void AnimationInit() = 0;
		// �v���C���[�̌��ݍ��W����A���E�ǂ���ɋ��邩������o��
		virtual bool SearchPosition();
		virtual void PatrolOn();	// ����
		virtual void EscapeOn();	// ����
		virtual void AttackOn();	// �U���J�n
		virtual bool Hit();	// �I�u�W�F�N�g�ƐڐG�������ǂ���
		virtual bool IsStandChip();
		virtual void Death();
		virtual void ChangeIdol();

		void ChangeState(ActionState nextstate, std::string key);
		// ���\�͈͂̎Z�o
		AABB VitalPart(Collision& col, int vital = 0);
		AABB DamageBox(int fix = 0);	// �_���[�W�{�b�N�X�̎Z�o

	public:
		EnemyBase(Game& game);
		~EnemyBase() override;

		virtual void Init() override;
		void Draw() override;
		
		inline EnemyType GetEnemyType() { return _eType; }
		inline ActionState GetActionState() { return _aState; }
		inline Vector2 GetMoveVector() override { return _moveVector; }

		virtual void CollisionHit(const std::string ckey, Collision acollision, bool direction);

		bool IsEmpty();

		virtual void SetParameter(ObjectValue objValue) override;	// parameter

		// virtual void Process() override;
		// virtual void Draw() override;
	};
}

