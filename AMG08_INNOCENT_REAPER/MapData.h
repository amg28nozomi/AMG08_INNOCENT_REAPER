/*****************************************************************//**
 * \file   MapData.h
 * \brief  �}�b�v�f�[�^�N���X
 *		   �}�b�v���
 * 
 *		   �}�b�v�f�[�^�}�l�[�W���[�N���X
 *		   �}�b�v���̊Ǘ����s��
 * 
 * \author ��؊�C
 * \date   October 2021
 *********************************************************************/
#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>

using std::pair;
using std::tuple;

namespace inr {
	// ��d�C���N���[�h�h�~
	class Game;
	// �}�b�v�f�[�^
	class MapData {
	public:
		// �R���X�g���N�^
		MapData();
		// �R���X�g���N�^
		// ����1:�}�b�v�̒���(���@����)�@����2:�`�b�v��(�����@���@�c)�@����3:�}�b�v�`�b�v�̃T�C�Y(���@����)�@����4:���C���[��
		// ����5:�t�@�C�����@����6:�}�b�v�`�b�v�̔z�u��񂪊i�[���ꂽ�R���e�i�@����7:�}�b�v�����҂̓����蔻�肪�i�[���ꂽ�R���e�i
		MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType);
		// �f�X�g���N�^
		~MapData() = default;
		// �}�b�v�T�C�Y(��)�̎擾
		inline int MapSizeWidth() { return _mapSizeW; }
		// �}�b�v�T�C�Y(����)�̎擾
		inline int MapSizeHeight() { return _mapSizeH; }
		// ���`�b�v���̎擾
		inline int ChipCount() { return _chipCount; }
		// �}�b�v�T�C�Y�̕�(�`�b�v��)�̎擾
		inline int ChipCountWidth() { return _chipCountW; }
		// �}�b�v�T�C�Y�̍���(�`�b�v��)�̎擾
		inline int ChipCountHeight() { return _chipCountH; }
		// �}�b�v�`�b�v�̕��̎擾
		inline int ChipSizeWidth() { return _chipSizeW; }
		// �}�b�v�`�b�v�̍����̎擾
		inline int ChipSizeHeight() { return _chipSizeH; }
		// ���C���[�̎擾
		inline int MapSizeLayer() { return _mapSizeLayer; }
		// �t�@�C�����̎擾
		inline std::string FileName() { return _fileName; }
		// �w�肵���}�b�v�`�b�v�̔z�u�����擾(����:�擾�������v�f)
		int MapDatas(int element) { return _mapDatas[element]; }
		// �}�b�v�`�b�v�̔z�u���̎擾
		std::vector<int> MapDatas() { return _mapDatas; }
		// �����蔻��̎擾
		std::vector<int> ChipType() { return _chipType; }
		// �}�b�v�T�C�Y�̓o�^(����1:���@����2:����)
		void SetMapSize(int mapSizeW, int mapSizeH);
		// 
		void SetChipCount(int count, int width, int height);
		// �}�b�v�`�b�v�T�C�Y�̓o�^(����1:���@����2:����)
		void SetChipSize(int width, int height);
		// ���C���[�̓o�^(����:���C���[��)
		inline void SetMapSizeLayer(int layers) { _mapSizeLayer = layers; }
		// �t�@�C�����̓o�^(����:�t�@�C����)
		inline void SetFillName(std::string filename) { _fileName = filename; }
		// �z�u���̓o�^(����:�z�u��񂪓o�^���ꂽint�^�̓��I�z��)
		inline void SetMapDatas(std::vector<int> mapdatas) { _mapDatas = mapdatas; }
		// �����蔻��̓o�^(����:�����蔻�肪�o�^���ꂽint�^�̓��I�z��)
		inline void SetChipType(std::vector<int> chiptype) { _chipType = chiptype; }
	private:
		int _mapSizeW;					// �}�b�v�T�C�Y�̕�
		int _mapSizeH;					// �}�b�v�T�C�Y����
		int _chipCount;					// ���`�b�v��
		int _chipCountW;				// �}�b�v�T�C�Y�̕�(�`�b�v��)
		int _chipCountH;				// �}�b�v�`�b�v�̍���(�`�b�v��)
		int _chipSizeW;					// �}�b�v�`�b�v�̕�
		int _chipSizeH;					// �}�b�v�`�b�v�̍���
		int _mapSizeLayer;				// ���C���[
		std::string _fileName;			// �ǂݍ��݃t�@�C����
		std::vector<int> _mapDatas;		// �}�b�v�`�b�v�̔z�u���
		std::vector<int> _chipType;		// �����蔻�肪����}�b�v�`�b�v
	};

	// �}�b�v�f�[�^�Ǘ��p�̃N���X
	class MapDataManager {
	public:
		// �R���X�g���N�^(����:�Q�[���N���X�̎Q��)
		MapDataManager(Game& game);
		// �f�X�g���N�^
		~MapDataManager();
		// �}�b�v�����Ǘ�����R���e�i
		using JsonMapData = std::unordered_map<std::string, MapData>;

		void LoadStageMap(JsonMapData& jsonMapData);	// �o�^
		bool GetStageMap(const std::string stage, MapData& mdata);	// �w�肵���}�b�v�f�[�^�̓ǂݍ���(����1:�ǂݎ�肽���}�b�v�̃L�[�A����2:�l��������I�u�W�F�N�g)
		bool IsLoad(const std::string key);	// �L�[�͓o�^����Ă��邩�H
		// �R���e�i�̉��
		void StageMapClear();
	private:
		JsonMapData _maps;	// �}�b�v�����Ǘ�����R���e�i
		Game& _game;		// �Q�[���N���X�̎Q��
	};
}

