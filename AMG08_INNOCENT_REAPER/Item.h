/*****************************************************************//**
 * @file   Item.h
 * @brief  �A�C�e���N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>
#include "Particle_Image.h"
#include "Collision.h"
#include "ObjectValue.h"

namespace inr {
  /** �A�C�e���̃R���p�C�����萔 */
  namespace item {
    // �A�C�e��
    constexpr auto ITEM = "item_light"; //!< �L�[
    constexpr auto ITEM_IMAGE = 150;    //!< �A�C�e���̃T�C�Y
  }
  /** ��d�C���N���[�h�h�~ */
  class Game;
  /** �A�C�e�� */
  class Item {
  public:
    /**
     * @brief        �R���X�g���N�^
     * @param game   �Q�[���N���X�̎Q��
     */
    Item(Game& game);
    /**
     * @brief        �f�X�g���N�^
     */
    ~Item() = default;
    /**
     * @brief        ����������
     */
    void Init();
    /**
     * @brief        �X�V����
     */
    void Process();
    /**
     * @brief        �`�揈��
     */
    void Draw();
    /**
     * @brief        �I�u�W�F�N�g���̓o�^
     * @param ovalue �I�u�W�F�N�g���
     */
    void SetParameter(ObjectValue ovalue);
    /**
     * @brief        �I�u�W�F�N�g���̍X�V
     */
    void ObjValueUpdate();
    /**
     * @brief        ����t���O�̎擾
     * @return       ����t���O��Ԃ�
     */
    inline bool IsGet() { return _isGet; }
    /**
     * @brief        �����t���O�̎擾
     * @return       �����t���O��Ԃ�
     */
    inline bool IsDel() { return _del; }
    /**
     * @brief        �I�u�W�F�N�g���̎擾
     * @return       �I�u�W�F�N�g����Ԃ�
     */
    inline ObjectValue GetObjectValue() { return _oValue; }
  private:
    Game& _game;         //!< �Q�[���N���X�̎Q��
    ObjectValue _oValue; //!< �I�u�W�F�N�g���
    Collision _col;      //!< �����蔻��
    std::string _gkey;   //!< �摜�L�[
    Vector2 _position;   //!< ���W
    int _count;          //!< �J�E���^
    int _maxFrame;       //!< �A�j���[�V�����̕`��t���[����
    int _messageNo;      //!<�@�Ή����Ă���message�ԍ�
    bool _isGet;         //!< ���łɓ��肳��Ă��邩�H
    bool _del;           //!< �����t���O
  };
}