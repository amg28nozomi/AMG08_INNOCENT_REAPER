/*****************************************************************//**
 * @file   MapData.h
 * @brief  �}�b�v�f�[�^�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <tuple>
#include <string>
#include <vector>

using std::pair;
using std::tuple;

namespace inr {
  /** ��d�C���N���[�h�h�~ */
  class Game;
  /** �}�b�v�f�[�^ */
  class MapData {
  public:
    /**
     * @brief              �R���X�g���N�^
     */
    MapData();
    /**
     * @brief              �R���X�g���N�^
     * @param mapSize      �}�b�v�T�C�Y(����:���@�E��:����)
     * @param chipCount    �`�b�v��(0:�����@1:���@2:�c)
     * @param chipSize     �}�b�v�`�b�v�̃T�C�Y(����:���@�E��:����)
     * @param mapSizeLayer ���C���[��
     * @param fileName     �t�@�C����
     * @param mapData      �}�b�v�`�b�v�̔z�u��񂪊i�[���ꂽ�R���e�i
     * @param chipType     �}�b�v�`�b�v�̓����蔻�肪�i�[���ꂽ�R���e�i
     */
    MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType);
    /**
     * @brief              �f�X�g���N�^
     */
    ~MapData() = default;
    /**
     * @brief              �}�b�v�T�C�Y(��)�̎擾
     * @return             �}�b�v�T�C�Y(��)��Ԃ�
     */
    inline int MapSizeWidth() { return _mapSizeW; }
    /**
     * @brief              �}�b�v�T�C�Y(����)�̎擾
     * @return             �}�b�v�T�C�Y(����)��Ԃ�
     */
    inline int MapSizeHeight() { return _mapSizeH; }
    /**
     * @brief              ���`�b�v���̎擾
     * @return             ���`�b�v����Ԃ�
     */
    inline int ChipCount() { return _chipCount; }
    /**
     * @brief              �}�b�v�T�C�Y�̕�(�`�b�v��)�̎擾
     * @return             �}�b�v�T�C�Y�̕�(�`�b�v��)��Ԃ�
     */
    inline int ChipCountWidth() { return _chipCountW; }
    /**
     * @brief              �}�b�v�T�C�Y�̍���(�`�b�v��)�̎擾
     * @return             �}�b�v�T�C�Y�̍���(�`�b�v��)��Ԃ�
     */
    inline int ChipCountHeight() { return _chipCountH; }
    /**
     * @brief              �}�b�v�`�b�v�̕��̎擾
     * @return             �}�b�v�`�b�v�̕���Ԃ�
     */
    inline int ChipSizeWidth() { return _chipSizeW; }
    /**
     * @brief              �}�b�v�`�b�v�̍����̎擾
     * @return             �}�b�v�`�b�v�̍�����Ԃ�
     */
    inline int ChipSizeHeight() { return _chipSizeH; }
    /**
     * @brief              ���C���[���̎擾
     * @return             ���C���[����Ԃ�
     */
    inline int MapSizeLayer() { return _mapSizeLayer; }
    /**
     * @brief              �t�@�C�����̎擾
     * @return             �t�@�C������Ԃ�
     */
    inline std::string FileName() { return _fileName; }
    /**
     * @brief             �w�肵���}�b�v�`�b�v�̔z�u�����擾
     * @param element     �擾�������v�f
     * @return            �Ή�����}�b�v�`�b�v�̔z�u����Ԃ�
     */
    int MapDatas(int element) { return _mapDatas[element]; }
    /**
     * @brief             �}�b�v�`�b�v�̔z�u���̎擾
     * @return            �}�b�v�`�b�v�̔z�u����Ԃ�
     */
    std::vector<int> MapDatas() { return _mapDatas; }
    /**
     * @brief             �����蔻��̎擾
     * @return            �����蔻����i�[����R���e�i��Ԃ�
     */
    std::vector<int> ChipType() { return _chipType; }
    /**
     * @brief             �}�b�v�T�C�Y�̓o�^
     * @param mapSizeW    ��
     * @param mapSizeH    ����
     */
    void SetMapSize(int mapSizeW, int mapSizeH);
    /**
     * @brief             �`�b�v�J�E���g�̓o�^
     * @param count       �`�b�v��
     * @param width       �}�b�v�T�C�Y�̕�(�`�b�v��)
     * @param height      �}�b�v�T�C�Y�̍���(�`�b�v��)
     */
    void SetChipCount(int count, int width, int height);
    /**
     * @brief             �}�b�v�`�b�v�T�C�Y�̓o�^
     * @param width       �}�b�v�`�b�v�̕�
     * @param height      �}�b�v�`�b�v�̍���
     */
    void SetChipSize(int width, int height);
    /**
     * @brief             ���C���[�̓o�^
     * @param layers      ���C���[��
     */
    inline void SetMapSizeLayer(int layers) { _mapSizeLayer = layers; }
    /**
     * @brief             �t�@�C�����̓o�^
     * @param fileName    �t�@�C����
     */
    inline void SetFillName(std::string fileName) { _fileName = fileName; }
    /**
     * @brief             �z�u���̓o�^
     * @param mapDatas    �z�u��񂪓o�^���ꂽint�^�̓��I�z��
     */
    inline void SetMapDatas(std::vector<int> mapDatas) { _mapDatas = mapDatas; }
    /**
     * @brief             �����蔻��̓o�^
     * @param chipType    �����蔻�肪�o�^���ꂽint�^�̓��I�z��
     */
    inline void SetChipType(std::vector<int> chipType) { _chipType = chipType; }
  private:
    int _mapSizeW;              //!< �}�b�v�T�C�Y�̕�
    int _mapSizeH;              //!< �}�b�v�T�C�Y����
    int _chipCount;             //!< ���`�b�v��
    int _chipCountW;            //!< �}�b�v�T�C�Y�̕�(�`�b�v��)
    int _chipCountH;            //!< �}�b�v�`�b�v�̍���(�`�b�v��)
    int _chipSizeW;             //!< �}�b�v�`�b�v�̕�
    int _chipSizeH;             //!< �}�b�v�`�b�v�̍���
    int _mapSizeLayer;          //!< ���C���[
    std::string _fileName;      //!< �ǂݍ��݃t�@�C����
    std::vector<int> _mapDatas; //!< �}�b�v�`�b�v�̔z�u���
    std::vector<int> _chipType; //!< �����蔻�肪����}�b�v�`�b�v
  };
}