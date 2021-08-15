#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ObjectBase.h"
#include "MapData.h"
#include "Vector2.h"
#include "ChipHitCheck.h"

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
		bool Clamp(Vector2& pos);	// ���[���h���W���X�N���[�����W�ɏC��
		bool IsScrollX();	// ���X�N���[�����Ă��邩�H
		bool IsScrollY();	// �c�X�N���[�����Ă��邩�H

		// �Q�b�^�[
		inline std::pair<int, int> GetMapSize() { return _mapSize; }
		inline std::tuple<int, int, int> GetChipCOunt() { return _chipCount; }
		inline std::pair<int, int> GetChipSize() { return _chipSize; }
		inline int GetMapSizeLayer() { return _mapSizeLayer; }
		inline std::string& GetFillCHips() { 
			std::string* fillchips = &_fileChips;
			return *fillchips;
		}
		// ���[���h���W�̎擾
		inline Vector2 GetWorldPosition() { return _worldPosition; }
		// ���[���h���W�̈ړ��ʂ��擾
		inline Vector2 BeforeWorldPos() { return _worldPosition - _worldLast; }
		void WorldUpdate(Vector2 pos);	// ���[���h���W�̍X�V

	private:
		using MapList = std::unordered_map<std::string, MapData>;
		Game& _game;

		std::pair<int, int> _mapSize;	// �}�b�v�̃T�C�Y(�`�b�v���@first:width�Asecond:hight)
		std::tuple<int, int, int> _chipCount;

		std::pair<int, int> _chipSize;	// �`�b�v�̕`��T�C�Y(pixel)
		int _mapSizeLayer;	// ���C���[��
		std::string _fileChips;	// �`�b�v�f�[�^�̃t�@�C����

		Vector2 _worldPosition;	// ���[���h���W
		Vector2 _worldLast;	// �O�t���[���̃��[���h���W

		std::vector<int> _mapData;	// �}�b�v�`�b�v�̔z�u���
		std::vector<int> _mapChipsType;	// �����蔻��̂���}�b�v�`�b�v(���ʔԍ�)�o�^�p

		void WorldClanp();
		std::string StringFillLoad(std::string FillName);
		int TiledJsonLoad(std::string fillPath, std::string strFillName);

		MapList _maplist;
		std::unique_ptr<ChipHitCheck> _chipCheck;


		// �f�o�b�O�p
		/*AABB _debugAABB;*/
	};
}

