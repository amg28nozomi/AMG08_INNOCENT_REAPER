/*****************************************************************//**
 * @file   TutorialServer.h
 * @brief  �`���[�g���A���摜���Ǘ�����T�[�o
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <memory>
#include <vector>

namespace inr {
  /** �`���[�g���A����� */
  class TutorialImage;
  /** �`���[�g���A���T�[�o */
  class TutorialServer {
  public:
    /**
     * @brief        �R���X�g���N�^
     */
    TutorialServer();
    /**
     * @brief        �f�X�g���N�^
     */
    ~TutorialServer();
    /**
     * @brief        �X�V����
     */
    void Process();
    /**
     * @brief        �`�揈��
     * 
     */
    void Draw();
    /**
     * @brief        �R���e�i�̉��
     * @return       true��Ԃ�
     */
    bool Clear();
    /**
     * @brief        �`���[�g���A���摜�̓o�^
     * @param tImage �`���[�g���A���摜
     */
    void Add(std::unique_ptr<TutorialImage> tImage);
  private:
    bool _isActive;                                      //!< ������ԃt���O
    std::vector<std::unique_ptr<TutorialImage>> _images; //!< �摜�p�R���e�i
  };
}