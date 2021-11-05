/*****************************************************************//**
 * @file   Transition.h
 * @brief  �X�e�[�W�J�ڏ��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>

namespace inr {
  /** �X�e�[�W�J�ڏ�� */
  class Transition {
  public:
    /**
     * @brief           �R���X�g���N�^
     * @param stageKey  �J�ڃX�e�[�W�L�[
     * @param position  �J�ڌ�̍��W
     * @param direction �J�ڌ�̌���
     */
    Transition(std::string stageKey, Vector2 position, bool direction);
    /**
     * @brief           �f�X�g���N�^
     */
    ~Transition() = default;
    /**
     * @brief           �X�e�[�W�L�[�̎擾
     * @return          �X�e�[�W�L�[��Ԃ�
     */
    inline std::string NextKey() { return _nextKey; }
    /**
     * @brief           ���W�̎擾
     * @return          ���W��Ԃ�
     */
    inline Vector2 NextPosition() { return _position; }
    /**
     * @brief           �����̎擾
     * @return          ������Ԃ�
     */
    inline bool NextDirection() { return _direction; }
  private:
    std::string _nextKey; //!< �J�ڐ�̃L�[
    Vector2 _position;    //!< �J�ڌ�̍��W(���@)
    bool _direction;      //!< ����
  };
}