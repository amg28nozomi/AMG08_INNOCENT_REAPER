/*****************************************************************//**
 * @file   SoldierDoll.h
 * @brief  �\���W���[�h�[���i�G�l�~�[�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "EnemyBase.h"

namespace inr {
  /** �G�l�~�[�̃R���p�C�����萔 */
  namespace enemy {
    /** �\���W���[�h�[���̃R���p�C�����萔 */
    namespace soldier {
      constexpr auto ATTACK_SE = "sd_attack_se";       //!< �؂��SE
      constexpr auto ATTACK_VOICE = "sd_attack_voice"; //!< �؂�t����SE
      constexpr auto DOWN = "sd_down";                 //!< �|�ꂽ�ۂ�SE
      constexpr auto ESCAPE_SE = "sd_escape_se";       //!< ��������SE
      constexpr auto ESCAPE_VOICE = "sd_escape_voice"; //!< �������{�C�X
      constexpr auto MOVE = "sd_move";                 //!< ����
    }
  }
  constexpr auto SOLDIER_IMAGE_W = 160;                //!< �\���W���[�h�[���̕�
  constexpr auto SOLDIER_IMAGE_H = 160;                //!< �\���W���[�h�[���̍���
  /** �\���W���[�h�[�� */
  class SoldierDoll : public EnemyBase {
  public:
    /*
     * @brief             �R���X�g���N�^
     * @param game        �Q�[���N���X�̎Q��
     */
    SoldierDoll(Game& game);
    /**
     * @brief             �f�X�g���N�^
     */
    ~SoldierDoll() override;
    /**
     * @brief             ����������
     */
    void Init() override;
    /**
     * @brief             �X�V����
     */
    void Process() override;
    /**
     * @brief             �I�u�W�F�N�g���̓o�^
     * @param objValue    �I�u�W�F�N�g���
     */
    void SetParameter(ObjectValue objValue) override;
  private:
    /**
     * @brief             �ړ�����
     */
    void Move() override;
    /**
     * @brief             ���񏈗�
     */
    void Patrol();
    /**
     * @brief             ����J�n
     */
    void PatrolOn() override;
    /**
     * @brief             ��������
     */
    void Escape();
    /**
     * @brief             �����J�n
     */
    void EscapeOn() override;
    /**
     * @brief             �U���J�n
     */
    void AttackOn() override;
    /**
     * @brief             �U������
     */
    void Attack();
    /**
     * @brief             �ҋ@��Ԃ̐ݒ�
     * @param stay        �ҋ@����
     */
    void ChangeIdol(int stay) override;
    /**
     * @brief             ���W���̍X�V
     */
    void PositionUpdate() override;
    /**
     * @brief             ��Ԃ̐؂�ւ�
     */
    void StateUpdate();
    /**
     * @brief             �A�N�V����
     */
    void Action() override;
    /**
     * @brief             ���S����
     */
    void Death() override;
    /**
     * @brief             ���ʉ��̒�~����
     * @return            true��Ԃ�
     */
    bool StopSound();
    /**
     * @brief             ���݂̓����蔻��̎擾
     * @param key         ���݂̏�Ԃ�\���L�[
     * @return            �����k�ł͂Ȃ��ꍇ�͒ʏ�̓����蔻���Ԃ�
     *                    �����k�̏ꍇ�͔����k�p�̓����蔻���Ԃ�
     */
    AABB NowCollision(std::string key) override;
    /**
     * @brief             ���@�A�N�V�����{�b�N�X�Ƃ̏Փ˔���
     * @param cKey        �Ώۂ̌��݂̏��(�L�[)
     * @param aCollision  �Ώۂ̓����蔻��{�b�N�X
     * @param direction   �Ώۂ̌����t���O
     */
    void CollisionHit(const std::string cKey, Collision aCollision, bool direction) override;
  };
}