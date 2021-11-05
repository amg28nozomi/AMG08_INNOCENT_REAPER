/*****************************************************************//**
 * @file   StageUi.h
 * @brief  �X�e�[�WUI�i�p�[�e�B�N���摜�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Particle_Image.h"
#include <vector>

namespace inr {
  /** �X�e�[�WUI */
  class StageUi : public Particle_Image {
  public:
    /**
     * @brief          �R���X�g���N�^
     * @param game     �Q�[���N���X�̎Q��
     */
    StageUi(Game& game);
    /**
     * @brief          �f�X�g���N�^
     */
    ~StageUi() = default;
    /**
     * @brief          ����������
     */
    void Init() override;
    /**
     * @brief          �X�V����
     */
    void Process() override;
    /**
     * @brief          �`�揈��
     */
    void Draw() override;
    /**
     * @brief          UI���̓o�^
     * @param graph    �摜�L�[
     * @param pos      ���W
     * @param pal      �����x
     */
    void SetParameter(std::string graph, Vector2 pos, int pal = 0);
    /**
     * @brief          �`��A�C�R���̐؂�ւ�
     * @param stageKey �X�e�[�W
     * @return         �A�C�R���ԍ���Ԃ�
     */
    bool ChangeNumber(const std::string stageKey);
    /**
     * @brief          �҂����Ԃ̃J�E���g����
     * @return         ���Z�����ɐ؂�ւ����ꍇ��true��Ԃ�
     *                 ����ȊO�̏ꍇ��false��Ԃ�
     */
    bool IntervalCount();
    /**
     * @brief          �`��̊J�n
     * @return         �J�n�ɐ��������ꍇ��true��Ԃ�
     *                 ���s�����ꍇ��false��Ԃ�
     */
    bool DrawStart() override;
    /**
     * @brief          �`�揈���t���O�̎擾
     * @return         �`�揈���t���O��Ԃ�
     */
    inline bool FadeDraw() { return _fadeDraw; }
  private:
    int _gNumber;      //!< ���Ԗڂ̉摜��`�悷�邩
    int _alive;        //!< �����t���[��
    bool _fadeDraw;    //!< �`�揈�������s������
  };
}