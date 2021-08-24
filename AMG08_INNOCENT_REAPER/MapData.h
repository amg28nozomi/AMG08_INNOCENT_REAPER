#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>

using std::pair;
using std::tuple;

namespace inr {

	class Game;

	class MapData {
	public:
		MapData(pair<int, int> mapSize, tuple<int, int, int> chipCount, pair<int, int> chipSize, int mapSizeLayer, std::string fileName, std::vector<int> mapData, std::vector<int> chipType);
		~MapData() = default;

		inline pair<int, int> GetMapSize() { return std::make_pair(_mapSizeW, _mapSizeH); }
		inline tuple<int, int, int> GetChipCount() { return std::make_tuple(_chipCount, _chipSizeW, _chipSizeH); }
		inline pair<int, int> GetChipSize() { return std::make_pair(_chipSizeW, _chipSizeH); }
		inline int GetMapSizeLayer() { return _mapSizeLayer; }
		inline std::string& GetFileName() { return _fileName; }

		void SetMapSize(int mapSizeW, int mapSizeH);
		void SetChipCount(int count, int width, int height);
		void SetChipSize(int width, int height);
		inline void SetMapSizeLayer(int layers) { _mapSizeLayer = layers; }
		inline void SetFillName(std::string filename) { _fileName = filename; }
		inline void SetMapDatas(std::vector<int> mapdatas) { _mapDatas = mapdatas; }
		inline void SetChipType(std::vector<int> chiptype) { _chipType = chiptype; }

	private:
		int _mapSizeW;		// �}�b�v�T�C�Y�̕�
		int _mapSizeH;		// �}�b�v�T�C�Y����
		int _chipCount;		// ���`�b�v��
		int _chipCountW;	// 
		int _chipCountH;	//  
		int _chipSizeW;		// 
		int _chipSizeH;		// 
		int _mapSizeLayer;	// 
		std::string _fileName;	// �ǂݍ��݃t�@�C����
		std::vector<int> _mapDatas;	// �}�b�v�`�b�v�̔z�u���
		std::vector<int> _chipType;	// �����蔻�肪����}�b�v�`�b�v
	};

	// �}�b�v�f�[�^�Ǘ��p�̃N���X
	class MapDataManager {
	public:
		MapDataManager(Game& game);
		~MapDataManager();

		using JsonMapData = std::unordered_map<std::string, MapData>;

		void LoadStageMap(JsonMapData& jsonMapData);	// �o�^
		bool GetStageMap(const std::string stage, MapData& mdata);	// �w�肵���}�b�v�f�[�^�̓ǂݍ���(����1:�ǂݎ�肽���}�b�v�̃L�[�A����2:�l��������I�u�W�F�N�g)
		void StageMapClear();	// �A�z�z��̏�����
	private:
		JsonMapData _maps;	// �}�b�v�̏���ۑ�
		Game& _game;
	};
}

