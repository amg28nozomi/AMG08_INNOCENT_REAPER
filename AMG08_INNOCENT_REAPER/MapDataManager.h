/*****************************************************************//**
 * @file   MapDataManager.h
 * @brief  �}�b�v���̊Ǘ����s���}�b�v�f�[�^�}�l�[�W��
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <string>

namespace inr {
  /** ��d�C���N���[�h�h�~ */
  class Game;
  class MapData;
  /** �}�b�v�f�[�^�Ǘ��p�̃N���X */
  class MapDataManager {
  public:
    /**
     * @brief             �R���X�g���N�^
     * @param game        �Q�[���N���X�̎Q��
     */
    MapDataManager(Game& game);
    /**
     * @brief             �f�X�g���N�^
     */
    ~MapDataManager();
    /** �}�b�v�����Ǘ�����R���e�i */
    using JsonMapData = std::unordered_map<std::string, MapData>;
    /**
     * @brief             �}�b�v���̈ꊇ�ǂݍ���
     * @param jsonMapData �}�b�v�����i�[�����A�z�z��
     */
    void LoadStageMap(JsonMapData& jsonMapData);
    /**
     * @brief             �w�肵���}�b�v���̎擾
     * @param stage       �X�e�[�W�L�[
     * @param mapData     �}�b�v���
     * @return            �ǂݍ��݂ɐ��������ꍇ��true��Ԃ�
     *                    �}�b�v���̎��o���Ɏ��s�����ꍇ��false��Ԃ�
     */
    bool GetStageMap(const std::string stage, MapData& mapData);
    /**
     * @brief             �ǂݍ��ݏ������s�����̔���
     * @param key         �X�e�[�W�L�[
     * @return            �ǂݍ��݂��s���ꍇ��true��Ԃ�
     *                    ���ɓǂݍ���ł���ꍇ��false��Ԃ�
     */
    bool IsLoad(const std::string key);
    /**
     * @brief             �R���e�i�̊J��
     */
    void StageMapClear();
  private:
    JsonMapData _maps; //!< �}�b�v�����Ǘ�����R���e�i
    Game& _game;       //!< �Q�[���N���X�̎Q��
  };
}