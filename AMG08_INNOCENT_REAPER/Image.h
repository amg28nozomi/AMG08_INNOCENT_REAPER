/*****************************************************************//**
 * @file   Image.h
 * @brief  �摜�̃X�[�p�[�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Vector2.h"
#include <string>

namespace inr {
  /** �w�i�̃R���p�C�����萔 */
  namespace background {
    // �w�i�f�ނ̊e����
    // ����̍Ւd
    constexpr auto BACK_GROUND_S = "back_groundS"; //!< ����̍Ւd�̉摜�L�[
    constexpr auto ALTAR = "back_altar";           //!< ����̍Ւd(�O�i)�̉摜�L�[
    constexpr auto STAGES_HEIGHT = 2160;           //!< ����̍Ւd�̃T�C�Y
    // �r�ꂽ��
    constexpr auto BACK_GROUND_1 = "back_ground1"; //!< �r�ꂽ���̉摜�L�[
    constexpr auto REMAINS = "back_remains";       //!< �r�ꂽ��(�O�i)�̉摜�L�[
    constexpr auto STAGE1_MAP_HEIGHT = 2160;       //!< �r�ꂽ���̃T�C�Y
    constexpr auto REMAINS_WIDTH = 600;            //!< �r�ꂽ��(�O�i)�̕�
    // �_��̐X
    constexpr auto BACK_GROUND_2 = "back_ground2"; //!< �_��̐X�摜�L�[
    constexpr auto STAGE2_MAP_HEIGHT = 2160;       //!< �_��̐X�̃T�C�Y
    // �{�X�X�e�[�W
    constexpr auto BACK_GROUND_B = "back_groundB"; //!< �{�X�X�e�[�W�̉摜�L�[
  }
  /** �G���f�B���O�̃R���p�C�����萔 */
  namespace end {
    // �X�^�b�t���[��
    constexpr auto STAFF_ROLL = "staff_roll";      //!< �X�^�b�t���[���̉摜�L�[    
    constexpr auto STAFF_ROLL_WIDTH = 1920;        //!< �X�^�b�t���[���̕�
    constexpr auto STAFF_ROLL_HEIGHT = 11880;      //!< �X�^�b�t���[���̍���
  }
  // �A�j���[�V�����f��
  // �^�C�g�����S
  constexpr auto TITLE_LOGO = "title_rogo";        //!< �^�C�g�����S�̉摜�L�[
  constexpr auto TITLE_IMAGE_W = 1920;             //!< �^�C�g�����S�̕�
  constexpr auto TITLE_IMAGE_H = 1080;             //!< �^�C�g�����S�̍���
  // �`�[�����S
  constexpr auto TEAM_LOGO = "team_logo";          //!< �`�[�����S�̉摜�L�[
  constexpr auto TEAM_IMAGE = 140;                 //!< �`�[�����S�̉摜�T�C�Y
  // �^�C�g���w�i
  constexpr auto TITLE_BG = "title_back_ground";   //!< �^�C�g���w�i�̉摜�L�[
  constexpr auto TITLE_IMAGE_WIDTH = 1920;         //!< �^�C�g���w�i�̕�
  constexpr auto TITLE_IMAGE_HEIGHT = 1080;        //!< �^�C�g���w�i�̍���
  // �e��^�C�g��UI�̉摜�L�[
  constexpr auto TITLE_START1 = "ui_start1";       //!< �X�^�[�gUI(1)�̉摜�L�[
  constexpr auto TITLE_START2 = "ui_start2";       //!< �X�^�[�gUI(2)�̉摜�L�[
  constexpr auto TITLE_EXIT1 = "ui_exit1";         //!< �I��UI(1)�̉摜�L�[
  constexpr auto TITLE_EXIT2 = "ui_exit2";         //!< �I��UI(2)�̉摜�L�[
  // �^�C�g��UI�̃T�C�Y
  constexpr auto TITLE_UI_HEIGHT = 166;            //!< �^�C�g��UI�̍���
  constexpr auto TITLE_START_WIDTH = 372;          //!< �X�^�[�gUI�̕�
  constexpr auto TITLE_EXIT_WIDTH = 298;           //!< �I��UI�̕�
  /** ��d�C���N���[�h�h�~ */
  class Game;
  /** �摜�̃X�[�p�[�N���X */
  class Image {
  protected:
    Game& _game;           //!<�@�Q�[���Q��
    Vector2 _pos;          //!< ���W
    std::string _graphKey; //!< �摜�L�[
  public:
    /**
     * @brief      �R���X�g���N�^
     * @param game �Q�[���N���X�̎Q��
     */
    Image(Game& game);
    /**
     * @brief      �f�X�g���N�^
     */
    ~Image() = default;
    /**
     * @brief      ����������
     */
    virtual void Init();
    /**
     * @brief      �X�V����
     */
    virtual void Process();
    /**
     * @brief      �`�揈��
     */
    virtual void Draw();
    /**
     * @brief      �t���O�̋N��
     */
    virtual void FlagOn();
  };
}