/*****************************************************************//**
 * @file   GimmickValue.h
 * @brief   �M�~�b�N���N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>

namespace inr {
  /** �I�u�W�F�N�g���̃R���p�C�����萔 */
  namespace oscenario {
    /** �M�~�b�N�̃R���p�C�����萔 */
    namespace gimmick {
      constexpr auto TYPE_NULL = -1;     //!< �Y���Ȃ�
      constexpr auto TYPE_LEVER = 0;     //!< ���o�[
      constexpr auto TYPE_CRYSTAL = 1;   //!< ����
      constexpr auto TYPE_BLOCK = 2;     //!< �����
      constexpr auto TYPE_DOOR = 3;      //!< �h�A
      // �M�~�b�N�t���O
      constexpr auto FLAG_NULL = -1;     //!< NULL
      constexpr auto FLAG_FALSE = 0;     //!< �M�~�b�N�t���O(�U)
      constexpr auto FLAG_TRUE = 1;      //!< �M�~�b�N�t���O(�^)
      /** �����̃R���p�C�����萔 */
      namespace crystal {
        constexpr auto TYPE_NULL = -1;   //!< �Y���Ȃ�
        constexpr auto TYPE_STAGE_0 = 0; //!< �X�e�[�W0�p
        constexpr auto TYPE_STAGE_2 = 1; //!< �X�e�[�W2�p
        // �h�A
        constexpr auto DOOR_NULL = -1;   //!< �Y���Ȃ�
        constexpr auto DOOR_RED = 0;     //!< �ԃh�A
        constexpr auto DOOR_BLUE = 1;    //!< �h�A
      }
    }
  }
  /** �M�~�b�N�̏�� */
  class GimmickValue {
  public:
    /**
     * @brief        �R���X�g���N�^
     * @param  type  �M�~�b�N�̎��
     * @param  flag  �M�~�b�N�̃t���O
     * @param  cType �����̎��
     * @param  types �h�A�̎��
     */
    GimmickValue(int type = oscenario::gimmick::TYPE_NULL, int flag = oscenario::gimmick::FLAG_NULL, int cType = oscenario::gimmick::crystal::TYPE_NULL, std::vector<int> types = { oscenario::gimmick::crystal::DOOR_NULL });
    /**
     * @brief        �M�~�b�N�^�C�v�̎擾
     * @return       �M�~�b�N�^�C�v��Ԃ�
     */
    inline int GimmickType() { return _gimmickType; }
    /**
     * @brief        �M�~�b�N�t���O�̎擾
     * @return       �M�~�b�N�t���O��Ԃ�
     */
    inline int GimmickFlag() { return _gimmickFlag; }
    /**
     * @brief        �����^�C�v�̎擾
     * @return       �����^�C�v��Ԃ�
     */
    inline int CrystalType() { return _crystalType; }
    /**
     * @brief        �h�A�̎擾
     * @return       �h�A��Ԃ�
     */
    inline std::vector<int> DoorType() { return _dtype; }
    /**
     * @brief        �M�~�b�N�t���O�̍X�V
     * @param flag   �t���O
     */
    inline void AddFlag(int flag) { _gimmickFlag = flag; }
  private:
    int _gimmickType;        //!< �M�~�b�N�̎�ށi-1:�ΏۊO�@0:���o�[�@1:�����@2:�@3:�h�A�j
    int _gimmickFlag;        //!< �M�~�b�N�̃t���O�i-1:�ΏۊO�@0:false�@1:true�j
    int _crystalType;        //!< �N���X�^���̎��(-1:�ΏۊO�A0:�X�e�[�W0�A1:�X�e�[�W2)
    std::vector<int> _dtype; //!< �h�A�̎��
  };
}