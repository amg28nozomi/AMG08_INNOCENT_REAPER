/*****************************************************************//**
 * @file   EnemyBase.h
 * @brief  �G�̃X�[�p�[�N���X
 *
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"

namespace inr {
	/** �G�̃R���p�C�����萔 */
	namespace enemy {
		// �摜�L�[
		constexpr auto SOLDIER_EMPTY = "soldier_empty";					//!< �����k(�\���W���[�h�[��)
		constexpr auto BIG_EMPTY = "big_empty";									//!< �����k(�r�b�O�h�[��)
		/** �Ԃ��� */
		namespace red {
			// �\���W���[�h�[��
			constexpr auto SOLDIER_WAKEUP = "soldier_r_wakeup";		//!< �N���オ��
			constexpr auto SOLDIER_IDOL = "soldier_r_stand";			//!< �ҋ@
			constexpr auto SOLDIER_PATROL = "soldier_r_patrol";		//!< ����
			constexpr auto SOLDIER_ATTACK = "soldier_r_attack";		//!< �U��
			// �r�b�O�h�[�� 
			constexpr auto BIG_WAKEUP = "big_r_wakeup";						//!< �N���オ��
			constexpr auto BIG_IDOL = "big_r_stand";							//!< �ҋ@
			constexpr auto BIG_PATROL = "big_r_patrol";						//!< ����
			constexpr auto BIG_TACKLE = "big_r_tackle";						//!< �^�b�N��
			constexpr auto BIG_HIPDROP = "big_r_hipdrop";					//!< �q�b�v�h���b�v
			/** �r�b�O�h�[���̃R���p�C�����萔 */
			namespace big {
				// �e�탂�[�V�����摜�̕�����
				constexpr auto WAKEUP_SIZE = 18;										//!< �N���オ�胂�[�V�����̑�������
				constexpr auto IDOL_SIZE = 11;											//!< �ҋ@���[�V�����̑�������
				constexpr auto MOVE_SIZE = 11;											//!< �ړ����[�V�����̑�������
				constexpr auto ATTACK_SIZE = 11;										//!< �U�����[�V�����̑�������
				constexpr auto HIPDROP_SIZE = 10;										//!< �q�b�v�h���b�v���[�V�����̑�������
			}
		}
		/** ���� */
		namespace blue {
			// �\���W���[�h�[���̉摜�L�[
			constexpr auto SOLDIER_WAKEUP = "soldier_b_wakeup";		//!< �N���オ��
			constexpr auto SOLDIER_IDOL = "soldier_b_stand";			//!< �ҋ@
			constexpr auto SOLDIER_PATROL = "soldier_b_patrol";		//!< ����
			constexpr auto SOLDIER_ESCAPE = "soldier_b_escape";		//!< ����
			// �r�b�O�h�[���̉摜�L�[
			constexpr auto BIG_WAKEUP = "big_b_wakeup";						//!< �N���オ��
			constexpr auto BIG_IDOL = "big_b_stand";							//!< �ҋ@
			constexpr auto BIG_PATROL = "big_b_patrol";						//!< ����
			constexpr auto BIG_ESCAPE = "big_b_escape";						//!< ����
			/** �r�b�O�h�[���̃R���p�C�����萔 */
			namespace big {
				// �e�탂�[�V�����摜�̑�������
				constexpr auto WAKEUP_SIZE = 18;										//!< �N���オ�胂�[�V�����̑�������
				constexpr auto IDOL_SIZE = 11;											//!< �ҋ@���[�V�����̑�������
				constexpr auto PATROL_SIZE = 11;										//!< ���񃂁[�V�����̑�������
				constexpr auto ESCAPE_SIZE = 16;										//!< �������[�V�����̑�������
			}
		}
		// �������̈ړ���
		constexpr auto ESCAPE_MAX = 1000;												//!< �������̑��ړ���
		constexpr auto ESCAPE_VECTOR = 120;											//!< �������̈ړ���(60F)
		// �U�����̈ړ���
		constexpr auto ATTACK_MAX = 1000;												//!< �U�����̑��ړ���
		constexpr auto ATTACK_VECTOR = 180;											//!< �U�����̈ړ���(60F)
		// ����
		constexpr auto MOVE_LEFT = true;												//!< ������
		constexpr auto MOVE_RIGHT = false;											//!< �E����
	}
	/** ��d�C���N���[�h�h�~ */
	class SoulSkin;
	/** �G�̃X�[�p�[�N���X */
	class EnemyBase : public ObjectBase {
	protected:
		/**
		 * @brief	�G�̎�ނ�\���񋓌^
		 *				�\���W���[�h�[���A�r�b�O�h�[���A�N���E�h�[��
		 */
		enum class EnemyType {
			SOLDIER_DOLL, BIG_DOLL, CROW_DOLL
		};
		/**
		 * @brief	�G�̏�Ԃ�\���񋓌^
		 *				�����k�A�ҋ@�A����A�U���A�q�b�v�h���b�v�A�����A�N���オ��
		 */
		enum class ActionState {
			EMPTY, IDOL, PATROL, ATTACK, HIPDROP, ESCAPE, WAKEUP
		};
		EnemyType _eType;									//!< �^�C�v
		ActionState _aState;							//!< ���
		boxs _collisions;									//!< �e�퓖���蔻��i�[�p
		AABB _searchBox;									//!< ���G�p�����蔻��
		Vector2 _moveVector;							//!< �ړ��x�N�g��
		std::shared_ptr<SoulSkin> _soul;	//!< ��
		int _aFrame;											//!< �A�N�V�������s�̂��߂̃t���[��
		int _aInterval;										//!< �A�N�V�����C���^�[�o��
		int _stay;												//!< �ҋ@�J�E���^
		double _actionX;									//!< �A�N�V��������x���ړ���
		double _patrolX;									//!< ���񎞂�x���ړ���
		bool _isAction;										//!< �A�N�V������ԂɈڍs���邩�̃t���O
		bool _drawStop;										//!< �`��X�V��~
		/**
		 * @brief								�A�j���[�V�����J�E���^�̊Ǘ�
		 */
		void AnimationCount() override;
		/**
		 * @brief								��ԑJ�ڏ���
		 * @param nextstate			�J�ڂ�����
		 * @param key						�J�ڌ�̍Đ��摜�L�[
		 */
		void ChangeState(ActionState nextstate, std::string key);
		/**
		 * @brief								���@�����G�͈͓��ɓ����Ă��邩�̔���
		 * @return							�͈͓��ɂ���ꍇ��true��Ԃ�
		 *											�͈͊O�ɂ���ꍇ��false��Ԃ�
		 */
		bool SearchPlayer();
		/**
		 * @brief								�ړ��������s���������z�֐�(��`�͔h����ōs��)
		 */
		virtual void Move() = 0;
		/**
		 * @brief								�A�N�V������ԂɈڍs���邩�̔���
		 */
		virtual void Action();
		/**
		 * @brief								���@�����E�ǂ��瑤�ɂ��邩�̔���
		 * @return							�����ɋ���ꍇ��true��Ԃ�
		 *											�E���ɋ���ꍇ��false��Ԃ�
		 */
		virtual bool SearchPosition();
		/**
		 * @brief								�����Ԃւ̑J�ڏ���
		 */
		virtual void PatrolOn();
		/**
		 * @brief								������Ԃւ̑J�ڏ���
		 */
		virtual void EscapeOn();
		/**
		 * @brief								�U����Ԃւ̑J�ڏ���
		 */
		virtual void AttackOn();
		/**
		 * @brief								�ڐG���Ă���}�b�v�`�b�v�Ɍ��ʂ����邩�̔���
		 * @return							���ʂ�����ꍇ��true��Ԃ�
		 *											���ʂ��Ȃ��ꍇ��false��Ԃ�
		 */
		virtual bool IsStandChip();
		/**
		 * @brief								���S����
		 */
		virtual void Death();
		/**
		 * @brief								�ҋ@��Ԃ̒���
		 * @param stay					�J�ڌ�̑҂�����
		 */
		virtual void ChangeIdol(int stay);
		/**
		 * @brief								���W���̊e��X�V����
		 */
		virtual void PositionUpdate();
		/**
		 * @brief								���ʉ��̍Đ�����
		 * @param skey					���ʉ��̃L�[
		 * @return							�Đ��ɐ��������ꍇ��true��Ԃ�
		 *											���s�����ꍇ��false��Ԃ�
		 */
		bool PlaySe(const std::string skey);
		/**
		 * @brief								���\�͈͂̎擾
		 * @param col						�Z�o�Ɏg�p���铖���蔻��{�b�N�X
		 * @param vital					���͈�(x)
		 * @return							�Z�o���������蔻��{�b�N�X��Ԃ�
		 */
		AABB VitalPart(Collision& col, int vital = 0);
		/**
		 * @brief								�U������{�b�N�X�̎Z�o�E�擾
		 * @param fix						����͈͂̏C���l(x)
		 * @return							�Z�o���������蔻��{�b�N�X��Ԃ�
		 */
		AABB DamageBox(int fix = 0);
	public:
		/**
		 * @brief								�R���X�g���N�^
		 * @param game					�Q�[���N���X�̎Q��
		 */
		EnemyBase(Game& game);
		/**
		 * @brief								�f�X�g���N�^
		 */
		~EnemyBase() override;
		/**
		 * @brief								����������
		 */
		virtual void Init() override;
		/**
		 * @brief								�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief								���@�A�N�V�����{�b�N�X�Ƃ̏Փ˔���
		 * @param	ckey					�Ώۂ̌��݂̏��(�L�[)
		 * @param	acollision		�Ώۂ̓����蔻��{�b�N�X
		 * @param	direction			�Ώۂ̌����t���O
		 */
		virtual void CollisionHit(const std::string ckey, Collision acollision, bool direction);
		/**
		 * @brief								�I�u�W�F�N�g���̓o�^
		 * @param objValue			�I�u�W�F�N�g���
		 */
		virtual void SetParameter(ObjectValue objValue) override;
		/**
		 * @brief								�����k���ǂ��̔���
		 * @return							�����k�̏ꍇ��true��Ԃ�
		 *											�����k�ł͂Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsEmpty();
		/**
		 * @brief								�ێ����Ă��鍰�̏��L����������A������ԂɈڍs������
		 * @return							���������ꍇ��true��Ԃ�
		 *											���s�����ꍇ��false��Ԃ�
		 */
		bool SoulPop();
		/**
		 * @brief								���̃G�l�~�[�̓{�X���ǂ���	
		 * @return							false��Ԃ�
		 */
		inline virtual bool IsBoss() { return false; }
		/**
		 * @brief								�^�C�v�̎擾
		 * @return							�^�C�v��Ԃ�
		 */
		inline EnemyType GetEnemyType() { return _eType; }
		/**
		 * @brief								��Ԃ̎擾
		 * @return							���݂̏�Ԃ�Ԃ�
		 */
		inline ActionState GetActionState() { return _aState; }
		/**
		 * @brief								�ړ��x�N�g���̎擾
		 * @return							�ړ��x�N�g����Ԃ�
		 */
		inline Vector2 GetMoveVector() override { return _moveVector; }
		/**
		 * @brief								�����k���̓����蔻��{�b�N�X���擾
		 * @return							�����k���̓����蔻��{�b�N�X��Ԃ�
		 */
		AABB EmptyBox();
	};
}

