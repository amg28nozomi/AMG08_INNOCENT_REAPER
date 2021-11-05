/*****************************************************************//**
 * @file   UI.h
 * @brief  UI�i�摜�N���X�̃T�u�N���X�j
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "Image.h"
#include <memory>
#include <vector>
#include <queue>

namespace inr {
  /** UI�̃R���p�C�����萔 */
  namespace ui {
    constexpr auto KEY_HP = "player_my_soul";     //!< HPUI
    constexpr auto KEY_RED = "player_red_soul";   //!< �Ԃ���
    constexpr auto KEY_BLUE = "player_blue_soul"; //!< ����
    constexpr auto KEY_BOX = "player_soul_box";   //!< �{�b�N�X
    constexpr auto HP_NUM = 5;                    //!< HPUI�̑�������
    constexpr auto HP_SIZE = 200;                 //!< HPUI�̉摜�T�C�Y
  }
  /** ��d�C���N���[�h�h�~ */
  class Player;
  class SoulSkin;
  /** UI�N���X */
  class UI : public Image {
  public:
    /**
     * @brief        �R���X�g���N�^
     * @param game   �Q�[���N���X�̎Q��
     */
    UI(Game& game);
    /**
     * @brief        �f�X�g���N�^
     */
    ~UI() = default;
    /**
     * @brief        ����������
     */
    void Init() override;
    /**
     * @brief        �X�V����
     */
    void Process() override;
    /**
     * @brief        �`�揈��
     */
    void Draw() override;
    /**
     * @brief        ���Z�b�g
     */
    void Reset();
    /**
     * @brief        ���@�|�C���^�̎擾
     */
    void PlayerUpdate();
  private:
    std::shared_ptr<Player> _player;               //!< ���@�̃|�C���^
    std::queue<std::shared_ptr<SoulSkin>> _uiSoul; //!< 1�t���[���O�̏��
    std::vector<std::string> _ghKeys;              //!< �摜�L�[
    std::vector<int> _count;                       //!< �A�j���[�V�����J�E���g
    std::vector<int> _pal;                         //!< �����x
    std::vector<bool> _active;                     //!< �Ώۂ͊��������Ă��邩�H
    /**
     * @brief        �摜�L�[�̎擾
     * @param number ���̐F
     * @return       �Ή������摜�L�[��Ԃ�
     */
    std::string GetGraphKey(int number);
    /**
     * @brief        �O���t�B�b�N�n���h���̎擾
     * @param key    �摜�L�[
     * @param count  ���݂̃J�E���g
     * @return       �擾�����O���t�B�b�N��Ԃ�
     *               ���s�����ꍇ��-1��Ԃ�
     */
    int GraphHandle(const std::string key, int count);
    /**
     * @brief        ���̃T�[�`
     */
    void SoulResearch();
    /**
     * @brief        ���@�̍��ۗL�����ς�������̔���
     * @param value  ���t���O
     * @return       �O�t���[���Ƃ̍�����Ԃ�
     */
    int IsSoulChange(bool value);
    /**
     * @brief        UI�̍폜
     */
    void Dels();
    /**
     * @brief        �J�E���^�̉��Z����
     */
    void ActiveCount();
    /**
     * @brief        UI�摜�̍X�V
     */
    void GraphUpdata();
    /**
     * @brief        UI�{�b�N�X�̕`�揈��
     */
    void DrawEmptyBox();
  };
}