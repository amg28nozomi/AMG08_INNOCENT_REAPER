/*****************************************************************//**
 * \file   EnemyBase.h
 * \brief  �G�̃X�[�p�[�N���X
 *
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "Collision.h"
namespace inr {
	namespace enemy {
		// �摜�L�[(ResourceServer)
		constexpr auto SOLDIER_EMPTY = "soldier_empty";			// �����k(�\���W���[�h�[��)
		constexpr auto BIG_EMPTY = "big_empty";					// �����k(�r�b�O�h�[��)
		// �Ԃ���
		namespace red {
			// �\���W���[�h�[��
			constexpr auto SOLDIER_WAKEUP = "soldier_r_wakeup";	// �N���オ��
			constexpr auto SOLDIER_IDOL = "soldier_r_stand";	// �A�C�h��
			constexpr auto SOLDIER_PATROL = "soldier_r_patrol";	// ����
			constexpr auto SOLDIER_ATTACK = "soldier_r_attack";	// �U��
			// �r�b�O�h�[�� 
			constexpr auto BIG_WAKEUP = "big_r_wakeup";			// �N���オ��
			constexpr auto BIG_IDOL = "big_r_stand";			// �ҋ@
			constexpr auto BIG_PATROL = "big_r_patrol";			// ����
			constexpr auto BIG_TACKLE = "big_r_tackle";			// �U���i�^�b�N���j
			constexpr auto BIG_HIPDROP = "big_r_hipdrop";		// �U���i�q�b�v�h���b�v�j

			namespace big {
				// �e�탂�[�V�����̕�����
				constexpr auto WAKEUP_SIZE = 5;
				constexpr auto IDOL_SIZE = 3;
				constexpr auto MOVE_SIZE = 5;
				constexpr auto ATTACK_SIZE = 5;
				constexpr auto HIPDROP_SIZE = 7;
				/*constexpr auto WAKEUP_SIZE = 17;
				constexpr auto IDOL_SIZE = 11;
				constexpr auto MOVE_SIZE = 13;
				constexpr auto ATTACK_SIZE = 11;
				constexpr auto HIPDROP_SIZE = 11;*/
			}
		}
		// ����
		namespace blue {
			// �\���W���[�h�[���̉摜�L�[
			constexpr auto SOLDIER_WAKEUP = "soldier_b_wakeup";	// �N���オ��
			constexpr auto SOLDIER_IDOL = "soldier_b_stand";	// �A�C�h��
			constexpr auto SOLDIER_PATROL = "soldier_b_patrol";	// ����
			constexpr auto SOLDIER_ESCAPE = "soldier_b_escape";	// ����
			// �r�b�O�h�[���̉摜�L�[
			constexpr auto BIG_WAKEUP = "big_b_wakeup";
			constexpr auto BIG_IDOL = "big_b_stand";
			constexpr auto BIG_PATROL = "big_b_patrol";
			constexpr auto BIG_ESCAPE = "big_b_escape";

			namespace big {
				// �e�탂�[�V�����̑�������
				constexpr auto WAKEUP_SIZE = 5;
				constexpr auto IDOL_SIZE = 3;
				constexpr auto PATROL_SIZE = 4;
				constexpr auto ESCAPE_SIZE = 5;
				/*constexpr auto WAKEUP_SIZE = 18;
				constexpr auto IDOL_SIZE = 11;
				constexpr auto PATROL_SIZE = 11;
				constexpr auto ESCAPE_SIZE = 13;*/
			}
		}
		// �������̈ړ���
		constexpr auto ESCAPE_MAX = 1000;						// �������̑��ړ���
		constexpr auto ESCAPE_VECTOR = 120;						// �������̈ړ���(60F)
		// �U�����̈ړ���
		constexpr auto ATTACK_MAX = 1000;						// �U�����̑��ړ���
		constexpr auto ATTACK_VECTOR = 180;						// �U�����̈ړ���(60F)
		// ����
		constexpr auto MOVE_LEFT = true;
		constexpr auto MOVE_RIGHT = false;
	}
	// ��d�C���N���[�h�h�~
	class SoulSkin;
	// �G�̊��N���X
	class EnemyBase : public ObjectBase {
	protected:
		// �G�̎��(�񋓌^)
		// �\���W���[�h�[���A�r�b�O�h�[���A�N���E�h�[��
		enum class EnemyType {
			SOLDIER_DOLL, BIG_DOLL, CROW_DOLL
		};
		// �G�̏��(�񋓌^)
		// �����k�A�ҋ@�A����A�U���A�q�b�v�h���b�v�A�����A�N���オ��
		enum class ActionState {
			EMPTY, IDOL, PATROL, ATTACK, HIPDROP, ESCAPE, WAKEUP
		};
		EnemyType _eType;					// ���
		ActionState _aState;				// ���
		boxs _collisions;					// �e�퓖���蔻��i�[�p
		AABB _searchBox;					// ���G�p�����蔻��
		Vector2 _moveVector;				// �ړ��x�N�g��
		std::shared_ptr<SoulSkin> _soul;	// ��
		int _aFrame;						// �A�N�V�������s�̂��߂̃t���[��
		int _aInterval;						// �A�N�V�����C���^�[�o��
		int _stay;							// �ҋ@�J�E���^
		double _actionX;					// �A�N�V��������x���ړ���
		double _patrolX;					// ���񎞂�x���ړ���
		bool _isSearch;						// �v���C���[��
		bool _isAction;						// �A�N�V������ԂɈڍs���邩�̃t���O
		bool _drawStop;						// �`��X�V��~
		// �A�j���[�V�����J�E���^�̊Ǘ�
		void AnimationCount() override;
		// ���@�����G�͈͓��ɓ��������̔���(true:�͈͓��ɂ���@false:���Ȃ�)
		bool SearchPlayer();
		// �ړ�����(�h����Œ�`)
		virtual void Move() = 0;
		// �A�N�V������ԂɈڍs���邩�̔���
		virtual void Action();
		// �v���C���[�̌��ݍ��W����A���E�ǂ���ɋ��邩������o��
		virtual bool SearchPosition();
		// �����Ԃւ̑J�ڏ���
		virtual void PatrolOn();
		// ������Ԃւ̑J�ڏ���
		virtual void EscapeOn();
		// �U����Ԃւ̑J�ڏ���
		virtual void AttackOn();
		// �����Ă���}�b�v�`�b�v�Ƃ̐ڐG����
		virtual bool IsStandChip();
		// ���S����
		virtual void Death();
		// �ҋ@��Ԃւ̒���(����;�J�ڌ�̑҂�����)
		virtual void ChangeIdol(int stay);
		// ���W���̍X�V����
		virtual void PositionUpdate();
		// �������̍Đ�(����:�Đ�������ʉ��̃L�[)
		bool PlaySe(const std::string skey);
		// ��ԑJ��(����1:�J�ڂ����ԁ@����2:�J�ڌ�̍Đ��摜)
		void ChangeState(ActionState nextstate, std::string key);
		// ���\�͈͂̎Z�o(����1:��ɂȂ铖���蔻��@����2:���\�ȉ���)
		AABB VitalPart(Collision& col, int vital = 0);
		// �U���͈͂̎Z�o(����:�C���l)
		AABB DamageBox(int fix = 0);
	public:
		// �R���X�g���N�^(����:�Q�[���N���X�̎Q��)
		EnemyBase(Game& game);
		// �f�X�g���N�^
		~EnemyBase() override;
		// ������
		virtual void Init() override;
		// �`��
		void Draw() override;
		// ��ނ̎擾
		inline EnemyType GetEnemyType() { return _eType; }
		// ��Ԃ̎擾
		inline ActionState GetActionState() { return _aState; }
		// �ړ��x�N�g���̎擾
		inline Vector2 GetMoveVector() override { return _moveVector; }
		// ���@�A�N�V�����Ƃ̏Փ˔���
		virtual void CollisionHit(const std::string ckey, Collision acollision, bool direction);
		// �����k���ǂ����̔���
		bool IsEmpty();
		// �{�X���ǂ����̔���
		inline bool IsBoss() { return _eType == EnemyType::CROW_DOLL; }
		// �ێ����Ă��鍰�𐶐�����
		bool SoulPop();
		// �I�u�W�F�N�g���̓o�^
		virtual void SetParameter(ObjectValue objValue) override;
		// �����k���̓����蔻��̎擾
		AABB EmptyBox();
	};
}

