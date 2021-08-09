#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>
#include "ObjectBase.h"
#include "MapData.h"

namespace inr {

	class Game;
	class ObjectBase;

	class MapChips {
	public:
		// �t�@�C������json�`���������ꍇ�̃R���X�g���N�^
		MapChips(Game& game, std::string& filePath, std::string& tiledFileName);
		~MapChips();

		void Init();	// �������B���[�h�ύX�E
		void Process();
		void Draw();

		int CheckHit(int x, int y);
		// �}�b�v�`�b�v�Ɠ������Ă��邩�ǂ����i�}�b�v�`�b�v�̏�ɗ����Ă��邩�ǂ����j
		bool IsHit(AABB box, double g);	// pos:�{�b�N�X�@mx:x�̈ړ������@my:y�̈ړ�����
		bool IsHit(AABB box, Vector2& pos, Vector2& move);	// �����o�������p

		// �Q�b�^�[
		inline std::pair<int, int> GetMapSize() { return _mapSize; }
		inline std::tuple<int, int, int> GetChipCOunt() { return _chipCount; }
		inline std::pair<int, int> GetChipSize() { return _chipSize; }
		inline int GetMapSizeLayer() { return _mapSizeLayer; }
		inline std::string& GetFillCHips() { 
			std::string* fillchips = &_fileChips;
			return *fillchips;
		}
		inline std::pair<int, int> GetScrPosition() { return _scrPosition; }
		inline void ScrUpdata(Vector2 pos) { _scrPosition = std::make_pair(pos.IntX(), pos.IntY()); }

	private:
		using MapList = std::unordered_map<std::string, MapData>;
		Game& _game;

		std::pair<int, int> _mapSize;	// �}�b�v�̃T�C�Y(�`�b�v���@first:width�Asecond:hight)
		std::tuple<int, int, int> _chipCount;

		std::pair<int, int> _chipSize;	// �`�b�v�̕`��T�C�Y(pixel)
		int _mapSizeLayer;	// ���C���[��
		std::string _fileChips;	// �`�b�v�f�[�^�̃t�@�C����
		std::pair<int, int> _scrPosition;	// �X�N���[�����W(����:x�@�E��:y)
		std::vector<int> _mapData;	// �}�b�v�`�b�v�̔z�u���
		std::vector<int> _mapChipsType;	// �����蔻��̂���}�b�v�`�b�v(���ʔԍ�)�o�^�p

		std::string StringFillLoad(std::string FillName);
		int TiledJsonLoad(std::string fillPath, std::string strFillName);

		MapList _maplist;


		// �f�o�b�O�p
		AABB _debugAABB;
	};
}

