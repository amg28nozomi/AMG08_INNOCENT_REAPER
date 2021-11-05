/*****************************************************************//**
 * @file   ObjectServer.h
 * @brief  �I�u�W�F�N�g�̊Ǘ����s���I�u�W�F�N�g�T�[�o
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "ObjectBase.h"
#include "Player.h"
#include "Vector2.h"
#include "Collision.h"

namespace inr{
  /** ��d�C���N���[�h�h�~ */
  class EnemyBase;
  class CrowDoll;
  class GimmickBase;
  /** �I�u�W�F�N�g�T�[�o */
  class ObjectServer {
  public:
    /**
     * @brief     �R���X�g���N�^
     */
    ObjectServer();
    /**
     * @brief     �f�X�g���N�^
     */
    ~ObjectServer();
    /**
     * @brief     �R���e�i�̏�����
     */
    void Clear();
    /**
     * @brief     �I�u�W�F�N�g�̓o�^
     * @param obj �I�u�W�F�N�g�̃V�F�A�[�h�|�C���^
     */
    void Add(std::shared_ptr<ObjectBase> obj);
    /**
     * @brief     �X�V����
     */
    void Process();
    /**
     * @brief     �`�揈��
     */
    void Draw();
    /**
     * @brief     �I�u�W�F�N�g�̊J������
     */
    void DeleteObject();
    /**
     * @brief     ���@�̎擾
     * @return    ���@�̃V�F�A�[�h�|�C���^��Ԃ�
     */
    std::shared_ptr<Player> GetPlayer();
    /**
     * @brief     �S�I�u�W�F�N�g�̎擾
     * @return    �S�I�u�W�F�N�g��Ԃ�
     */
    std::vector<std::shared_ptr<ObjectBase>> GetObjects() { return _objects; }
    /**
     * @brief     �S�G�l�~�[�̎擾
     * @return    �S�G�l�~�[��Ԃ�
     */
    std::vector<std::shared_ptr<EnemyBase>> GetEnemys();
    /**
     * @brief     ���@�͐�������Ă��邩�H
     * @return    ��������Ă���ꍇ��true��Ԃ�
     *            ��������Ă��Ȃ��ꍇ��false��Ԃ�
     */
    bool IsPlayer();
    /**
     * @brief     ���@�̃I�u�W�F�N�g���폜
     */
    void ObjectsClear();
    /**
     * @brief     �S�I�u�W�F�N�g�̉��
     */
    inline void AllClear() { _objects.clear(); }
    std::shared_ptr<SoulSkin> GetSoul();
    /**
     * @brief     ���������t���O�̋N��
     */
    inline void DelOn() { _delete = true; }
    /**
     * @brief     �����t���O�̎擾
     * @return    �����t���O��Ԃ�
     */
    inline bool DelFlag() { return _delete; }
  private:
    bool _updateFlg;                                   //!< �X�V�����邩
    bool _delete;                                      //!< �����t���O
    std::vector<std::shared_ptr<ObjectBase>> _objects; //!< �I�u�W�F�N�g�i�[�p�R���e�i
    std::vector<std::shared_ptr<ObjectBase>> _addObj;  //!< �o�^�\��
    std::vector<std::shared_ptr<ObjectBase>> _delObj;  //!< �����\��
  };
}