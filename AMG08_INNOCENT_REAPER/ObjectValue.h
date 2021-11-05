/*****************************************************************//**
 * @file   ObjectValue.h
 * @brief  �I�u�W�F�N�g���N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <vector>
#include "Vector2.h"
#include "GimmickValue.h"

namespace inr {
  /** �I�u�W�F�N�g���̃R���p�C�����萔 */
  namespace oscenario {
    constexpr auto OBJ_NULL = -1;        //!< �Y���Ȃ�
    constexpr auto OBJ_PLAYER = 0;       //!< ���@
    constexpr auto OBJ_SOLDIER_DOLL = 1; //!< �\���W���[�h�[��
    constexpr auto OBJ_BIG_DOLL = 2;     //!< �r�b�O�h�[��
    constexpr auto OBJ_CROW_DOLL = 3;    //!< �N���E�h�[��
    constexpr auto OBJ_SOUL = 4;         //!< ��
    constexpr auto OBJ_LEVER = 5;        //!< ���o�[
    constexpr auto OBJ_BLOCK = 6;        //!< �����
    constexpr auto OBJ_CRYSTAL = 7;      //!< ����
    constexpr auto OBJ_DOOR = 8;         //!< �h�A
    constexpr auto OBJ_ITEM = 9;         //!< �A�C�e��
    /** �I�u�W�F�N�g�^�C�v�̃R���p�C�����萔 */
    namespace type {
      constexpr auto PLAYER = 0;         //!< ���@
      constexpr auto ENEMY = 1;          //!< �G
      constexpr auto SOUL = 2;           //!< ��
      constexpr auto GIMMICK = 3;        //!< �M�~�b�N
      constexpr auto ITEM = 4;           //!< �A�C�e��
    }
  }
  /** �I�u�W�F�N�g��� */
  class ObjectValue {
  public:
    /**
     * @brief  �R���X�g���N�^
     */
    ObjectValue();
    /**
     * @brief           �R���X�g���N�^
     * @param classType ��������N���X
     * @param xy        �����n�_
     * @param direction �����t���O
     * @param soulColor ���̐F(0:�Ȃ��@1:�ԁ@2:��)
     * @param gValue    �M�~�b�N���
     */
    ObjectValue(int classType, Vector2 xy, bool direction = false, int soulColor = 0, GimmickValue gValue = GimmickValue());
    /**
     * @brief           �R���X�g���N�^
     * @param classType ��������N���X
     * @param xy        �����n�_���i�[�����R���e�i
     * @param direction �����t���O
     * @param soulColor ���̐F(0:�Ȃ��@1:�ԁ@2:��)
     * @param gValue    �M�~�b�N���
     */
    ObjectValue(int classType, std::vector<Vector2> xy, bool direction = false, int soulColor = 0, GimmickValue gValue = GimmickValue());
    /**
     * @brief           �f�X�g���N�^
     */
    ~ObjectValue() = default;
    /**
     * @brief           �N���X�ԍ��̎擾
     * @return          �N���X�ԍ���Ԃ�
     */
    inline int ClassName() { return _class; }
    /**
     * @brief           �����̎擾
     * @return          ������Ԃ�
     */
    inline int SoulType() { return _soulType; }
    /**
     * @brief           �M�~�b�N�^�C�v�̎擾
     * @return          �M�~�b�N�^�C�v��Ԃ�
     */
    inline int GimmickType() { return _gimmick.GimmickType(); }
    /**
     * @brief           �M�~�b�N�t���O�̎擾
     * @return          �M�~�b�N�t���O��Ԃ�
     */
    inline int GimmickFlag() { return _gimmick.GimmickFlag(); }
    /**
     * @brief           �����^�C�v�̎擾
     * @return          �����^�C�v��Ԃ�
     */
    inline int CrystalType() { return _gimmick.CrystalType(); }
    /**
     * @brief           �I�u�W�F�N�g�^�C�v�̎擾
     * @return          �I�u�W�F�N�g�^�C�v��Ԃ�
     */
    inline int ObjectType() { return _objType; }
    /**
     * @brief           �����t���O�̎擾
     * @return          �����t���O��Ԃ�
     */
    inline bool Direction() { return _direction; }
    /**
     * @brief           �o�����W�̎擾
     * @return          �o�����W��Ԃ�
     */
    inline std::vector<Vector2> Positions() { return _spawnPos; }
    /**
     * @brief           �h�A�^�C�v�̎擾
     * @return          �h�A�^�C�v��Ԃ�
     */
    inline std::vector<int> DoorType() { return _gimmick.DoorType(); }
    /**
     * @brief           ���W�̍X�V
     * @param newPos    �X�V��̍��W
     */
    void PositionsUpdate(Vector2 newPos);
    /**
     * @brief           �M�~�b�N�t���O�ƍ����̍X�V
     * @param flag      �t���O
     * @param soul      ��
     */
    void FlagUpdate(int flag, int soul = 0);
    /**
     * @brief           ����Ԃ̍X�V
     * @param soul      ��
     */
    inline void SoulUpdate(int soul) { _soulType = soul; }
    /**
     * @brief            �����t���O�̍X�V
     * @param newDir     ����
     */
    inline void DirectionUpdate(bool newDir) { _direction = newDir; }
  private:
    int _class;                     //!< ��������N���X�͉���
    int _objType;                   //!< �I�u�W�F�N�g�̎��
    int _soulType;                  //!< ���̗L���i0:�������@1:�ԁ@2:�j
    std::vector<Vector2> _spawnPos; //!< �o���n�_
    bool _direction;                //!< ����
    GimmickValue _gimmick;          //!< �M�~�b�N�̓o�^���
    /**
     * @brief           �I�u�W�F�N�g�^�C�v�̔���
     * @param className �N���X�ԍ�
     * @return          �I�u�W�F�N�g�^�C�v��Ԃ�
     */
    int IsObjectType(int className);
  };
}