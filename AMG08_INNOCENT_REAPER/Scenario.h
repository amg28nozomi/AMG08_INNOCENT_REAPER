/*****************************************************************//**
 * @file   Scenario.h
 * @brief  �V�i���I�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "Vector2.h"

namespace inr {
  /** ��d�C���N���[�h�h�~ */
  class Game;
  class Player;
  class EnemyBase;
  class ObjectValue;
  class ImageValue;
  class TutorialValue;
  class Particle_Image;
  class TutorialImage;
  /** �摜�����i�[����A�z�z�� */
  using ImageMap = std::unordered_map<int, ImageValue>;
  /** �V�i���I */
  class Scenario {
  public:
    /** �I�u�W�F�N�g�����i�[����A�z�z��(�L�[:�X�e�[�W�@�l:�I�u�W�F�N�g���̃R���e�i) */
    using ScenarioMap = std::unordered_map<std::string, std::vector<ObjectValue>>;
    /**
     * @brief         �R���X�g���N�^
     * @param game    �Q�[���N���X�̎Q��
     */
    Scenario(Game& game);
    /**
     * @brief         �f�X�g���N�^
     */
    ~Scenario();
    /**
     * @brief         ����������
     */
    void Init();
    /**
     * @brief         �V�i���I�̍X�V
     * @param key     �X�e�[�W
     */
    void ScenarioUpdate(std::string key);
    /**
     * @brief         �I�u�W�F�N�g�̐���
     * @param key     �X�e�[�W
     * @return        true��Ԃ�
     */
    bool AddObjects(const std::string key);
    /**
     * @brief         �{�X��J�n���Ƀu���b�N�𐶐�����
     */
    void BossBlock();
  private:
    Game& _game;            //!< �Q�[���N���X�̎Q��
    ScenarioMap _scenarios; //!< �e�X�e�[�W�̃I�u�W�F�N�g���
    /**
     * @brief         ���@�̐���
     */
    void AddPlayer();
    /**
     * @brief         �\���W���[�h�[���̐���
     * @param oValue  �I�u�W�F�N�g���
     */
    void AddSoldierDoll(ObjectValue oValue);
    /**
     * @brief         �r�b�O�h�[���̐���
     * @param oValue  �I�u�W�F�N�g���
     */
    void AddBigDoll(ObjectValue oValue);
    /**
     * @brief         �N���E�h�[���̐���
     * @param oValue  �I�u�W�F�N�g���
     */
    void AddCrowDoll(ObjectValue oValue);
    /**
     * @brief         ���o�[�̐���
     * @param oValue  �I�u�W�F�N�g���
     */
    void AddLever(ObjectValue oValue);
    /**
     * @brief         �����̐���
     * @param oValue  �I�u�W�F�N�g���
     */
    void AddCrystal(ObjectValue oValue);
    /**
     * @brief         �����̐���
     * @param oValue  �I�u�W�F�N�g���
     */
    void AddBlock(ObjectValue oValue);
    /**
     * @brief         �h�A�̐���
     * @param oValue  �I�u�W�F�N�g���
     */
    void AddDoor(ObjectValue oValue);
    /**
     * @brief         �A�C�e���̐���
     * @param oValue  �I�u�W�F�N�g���
     */
    void AddItem(ObjectValue oValue);
    /**
     * @brief         �摜�̐����E�擾
     * @param pValue  �摜���
     * @return        �����������摜��Ԃ�
     */
    std::unique_ptr<Particle_Image> AddImage(ImageValue pValue);
    /**
     * @brief         �`���[�g���A���摜�̐���
     * @param key     ���������ꍇ��true��Ԃ�
     *                ����ȊO�̏ꍇ��false��Ԃ�
     */
    bool AddTutorials(const std::string key);
    /**
     * @brief         �I�u�W�F�N�g���̓o�^
     * @param key     �X�e�[�W
     * @param oValues �I�u�W�F�N�g���
     */
    void LoadObjectData(std::string key, std::vector<ObjectValue> oValues);  // ���ǂݍ���
    /**
     * @brief         �摜���̓o�^
     */
    void LoadImageData(ImageMap iMap);
    /**
     * @brief         �Ώۂ̃L�[�͓ǂݍ��񂾂��H
     * @param key     �X�e�[�W
     * @return        �o�^���s�����ꍇ��true��Ԃ�
     *                ���ɓo�^����Ă���ꍇ��false��Ԃ�
     */
    bool IsLoad(const std::string key);
    /**
     * @brief         �A�z�z��̏�����
     */
    void ClearScenario();
  };
}