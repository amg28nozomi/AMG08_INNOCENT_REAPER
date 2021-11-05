/*****************************************************************//**
 * @file   Pause_UI.h
 * @brief  �|�[�Y�pUI�N���X�i�p�[�e�B�N���摜�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Particle_Image.h"

namespace inr {
  /** �V�X�e���̃R���p�C�����萔 */
  namespace system {
    constexpr auto CURSOR_MOVE = "cursor_move"; //!< �J�[�\���摜
    // �摜�ԍ�
    constexpr auto CONTINUE = 0;                //!< �R���e�B�j���[
    constexpr auto CONTROLS = 1;                //!< �������
    constexpr auto QUIT_TO_TITLE = 2;           //!< �^�C�g���ɖ߂�
  }
  /** �|�[�Y�pUI */
  class Pause_UI : public Particle_Image {
  public:
    /**
     * @brief      �R���X�g���N�^
     * @param game �Q�[���N���X�̎Q��
     */
    Pause_UI(Game& game);
    /**
     * @brief      �f�X�g���N�^
     */
    ~Pause_UI() = default;
    /**
     * @brief      ����������
     */
    void Init() override;
    /**
     * @brief      �X�V����
     */
    void Process() override;
    /**
     * @brief      �摜����э��W�̕ύX
     * @param type �㉺���͏��
     */
    void ChangePosition(bool type) override;
    /**
     * @brief      �ړ��t���O�̎擾
     * @return     �ړ��t���O��Ԃ�
     */
    bool IsMove() { return _move; }
    /**
     * @brief      UI�ԍ��̎擾  
     * @return     UI�ԍ���Ԃ�
     */
    int UiNumber() { return _no; }
  private:
    int _no;       //!< UI�ԍ�
    int _upDown;   //!< �㉺�ǂ���Ɉړ����邩�i1:��A-1:���j
    double _nextY; //!< �ړ���
    bool _move;    //!< �ړ������ǂ����iTRUE�̎��̂݁A�ړ�������t�j
  };
}