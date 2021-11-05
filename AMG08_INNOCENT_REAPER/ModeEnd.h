/*****************************************************************//**
 * @file   ModeEnd.h
 * @brief  �G���f�B���O���Ǘ����郂�[�h�G���h�N���X�i���[�h�x�[�X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ModeBase.h"
#include "MoveImage.h"
#include <memory>

namespace inr {
  /** �G���f�B���O */
  class ModeEnd : public ModeBase {
  public:
    /**
     * @brief      �R���X�g���N�^
     * @param game �Q�[���N���X�̎Q��
     */
    ModeEnd(Game& game);
    /**
     * @brief      �f�X�g���N�^
     */
    ~ModeEnd() = default;
    /**
     * @brief      ����������
     */
    void Init() override;
    /**
     * @brief      �X�V����
     */
    void Process() override;
    /**
     * @brief      �`�揈��
     */
    void Draw() override;
  private:
    int _count;                            //!< �҂�����
    bool _end;                             //!< �������I�����邩�H
    std::unique_ptr<MoveImage> _staffRoll; //!< �X�^�b�t���[��
    /**
     * @brief      �Q�[���^�C�g���ɑJ�ڂ��邩�̔���
     * @return     �^�C�g���ɑJ�ڂ���ꍇ��true��Ԃ�
     * @return     �J�ڂ��Ȃ��ꍇ��false��Ԃ�
     */
    bool IsEnd();
  };
}