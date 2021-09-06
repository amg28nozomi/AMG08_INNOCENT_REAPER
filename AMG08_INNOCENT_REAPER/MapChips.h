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
#include "FileData.h"

namespace inr {

	namespace stage {
		// �X�e�[�W�ǂݍ��ݗp�̃L�[
		constexpr auto CHANGE_NULL = "change_null";	// �ʏ��Ԃ̃L�[�؂�ւ�

		constexpr auto STAGE_0 = "stage0";	// �`���[�g���A��
		constexpr auto STAGE_1 = "stage1";		// �X�e�[�W1(�p��)
		constexpr auto STAGE_2_1 = "stage2-1";	// �X�e�[�W2(�X��-����)
		constexpr auto STAGE_2_2 = "stage2-2";	// �X�e�[�W2(�X��-�㕔)
		constexpr auto STAGE_2_3 = "stage2-3";	// �X�e�[�W2(�X��-����)
		constexpr auto STAGE_3 = "boss_stage";	// �X�e�[�W3(�{�X��)

		constexpr auto KEY_NORMAL = "mapchip_normal";	// �ʏ�}�b�v�`�b�v
	}

	namespace mapchip {
		constexpr auto TYPE_NULL = -1;	// ���o�^�ԍ�
		constexpr auto TYPE_NORMAL = 0;	// �ʏ�
		constexpr auto TYPE_THORM = 1;	// ��
		constexpr auto TYPE_IVX = 2;	// ��
	}

	class Game;
	class ObjectBase;

	class MapChips {
	public:
		MapChips(Game& game);
		// �t�@�C������json�`���������ꍇ�̃R���X�g���N�^
		MapChips(Game& game, std::string& filePath, std::string& tiledFileName);
		~MapChips();

		void Init();	// �������B���[�h�ύX�E
		void Process();
		void Draw();

		int CheckHit(int x, int y);

		int IsStand(AABB box, Vector2& pos, double g, Vector2* lastpos);	// �}�b�v�`�b�v�̏�ɗ����Ă��邩�ǂ����H
		// �}�b�v�`�b�v�Ɠ������Ă��邩�ǂ����i�}�b�v�`�b�v�̏�ɗ����Ă��邩�ǂ����j
		// bool IsHit(AABB box, double g);	// pos:�{�b�N�X�@mx:x�̈ړ������@my:y�̈ړ�����
		int IsHit(AABB box, Vector2& pos, Vector2& move, bool direction, bool isUpdate);	// �����o�������p
		bool Clamp(Vector2& pos);	// ���[���h���W���X�N���[�����W�ɏC��
		bool IsScrollX();	// ���X�N���[�����Ă��邩�H
		bool IsScrollY();	// �c�X�N���[�����Ă��邩�H

		void ChangeMap();	// �}�b�v�̐؂�ւ�����

		// �Q�b�^�[
		inline int GetMapSizeWidth() { return _nowMap.MapSizeWidth() * _nowMap.ChipSizeWidth(); }
		inline std::pair<int, int> GetMapSize() { return std::make_pair(_nowMap.MapSizeWidth(), _nowMap.MapSizeHeight()); }
		inline std::tuple<int, int, int> GetChipCount() { return std::make_tuple(_nowMap.ChipCount(), _nowMap.ChipCountWidth(), _nowMap.ChipCountHeight()); }
		inline std::pair<int, int> GetChipSize() { return std::make_pair(_nowMap.ChipSizeWidth(), _nowMap.ChipSizeHeight()); }
		inline int GetMapSizeLayer() { return _nowMap.MapSizeLayer(); }
		inline std::string GetFileChips() { return _nowMap.FileName(); }
		// ���[���h���W�̎擾
		inline Vector2 GetWorldPosition() { return _worldPosition; }
		// ���[���h���W�̈ړ��ʂ��擾
		inline Vector2 BeforeWorldPos() { return _worldPosition - _worldLast; }
		void WorldUpdate(Vector2 pos);	// ���[���h���W�̍X�V

	private:
		Game& _game;

		MapData _nowMap;	// ���݂̃}�b�v�`�b�v���
		std::unique_ptr<MapDataManager> _mapManager;
		std::unique_ptr<ChipHitCheck> _chipCheck;

		std::string _skey;	// ���݂̃X�e�[�W
		std::string _nextStage;	// ���̃X�e�[�W

		std::string _chipKey;	// ���o���}�b�v�`�b�v�̃L�[

		std::vector<FileData> _sFiles;

		Vector2 _worldPosition;	// ���[���h���W
		Vector2 _worldLast;	// �O�t���[���̃��[���h���W

		void WorldClanp();
		std::string StringFillLoad(std::string FillName);
		// ����1:�o�^�p�L�[�@����2:�t�@�C���p�X�@����3:�t�@�C����
		int TiledJsonLoad(std::string skey, std::string filePath, std::string strFileName);
		void SetChipsMap();	// �e�X�e�[�W�̈ꕔ�}�b�v�`�b�v�̔���͈͂��C��
		void SetChipMember(); // member

		//std::pair<int, int> _mapSize;	// �}�b�v�̃T�C�Y(�`�b�v���@first:width�Asecond:hight)
		//std::tuple<int, int, int> _chipCount;

		//std::pair<int, int> _chipSize;	// �`�b�v�̕`��T�C�Y(pixel)
		//int _mapSizeLayer;	// ���C���[��
		//std::string _fileChips;	// �`�b�v�f�[�^�̃t�@�C����


		// �f�o�b�O�p
		/*AABB _debugAABB;*/
	};
}

