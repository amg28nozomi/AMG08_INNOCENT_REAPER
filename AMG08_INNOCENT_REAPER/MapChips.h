#pragma once
#include <tuple>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ObjectBase.h"
#include "MapData.h"
#include "MapDataManager.h"
#include "Vector2.h"
#include "ChipHitCheck.h"
#include "FileData.h"
#include "StageTransition.h"

namespace inr {
	/** �X�e�[�W�̃R���p�C�����萔 */
	namespace stage {
		// �}�b�v�`�b�v�̎��
		constexpr auto KEY_NORMAL = "mapchip_normal";		//!< �ʏ�}�b�v�`�b�v
	}
	/** �}�b�v�`�b�v�̃R���p�C�����萔 */
	namespace mapchip {
		// �}�b�v�`�b�v�̃^�C�v�ԍ�
		constexpr auto TYPE_NULL = -1;									//!< ���o�^
		constexpr auto TYPE_NORMAL = 0;									//!< �ʏ�`�b�v
		constexpr auto TYPE_THORM = 1;									//!< ���`�b�v
		constexpr auto TYPE_IVX = 2;										//!< �Ӄ`�b�v
	}
	/** ��d�C���N���[�h�h�~ */
	class Game;
	class ObjectBase;
	/** �}�b�v�`�b�v */
	class MapChips {
	public:
		/**
		 * @brief						�R���X�g���N�^
		 * @param game			�Q�[���N���X�̎Q��
		 */
		MapChips(Game& game);
		/**
		 * @brief						�f�X�g���N�^
		 */
		~MapChips();
		/**
		 * @brief						����������
		 */
		void Init();
		/**
		 * @brief						�X�V����
		 */
		void Process();
		/**
		 * @brief						�`�揈��
		 */
		void Draw();
		/**
		 * @brief						�`�b�v�ԍ��̎擾
		 * @param	x					x���̍��W(�}�b�v�`�b�v���Z)
		 * @param	y					y���̍��W(�}�b�v�`�b�v���Z)
		 * @return					�}�b�v�`�b�v������ꍇ�͊Y������`�b�v�ԍ���Ԃ�
		 *									�}�b�v�`�b�v���Ȃ��ꍇ��0��Ԃ�
		 */
		int CheckHit(int x, int y);
		/**
		 * @brief						�}�b�v�`�b�v�̏�ɗ����Ă��邩�̔���
		 * @param box				�Ώۂ̓����蔻��{�b�N�X
		 * @param pos				�Ώۂ̍��W
		 * @param g					�d�͒l
		 * @param lastpos		�Ō�ɗ����Ă������W
		 * @param flag			���@���ǂ����̃t���O
		 * @return					�ڐG���Ă���ꍇ�͑Ώۂ̌��ʔԍ���Ԃ�
		 *									�ڐG���Ă��Ȃ��ꍇ��-1��Ԃ�
		 */
		int IsStand(AABB box, Vector2& pos, double g, Vector2* lastpos, bool flag = false);
		/**
		 * @brief						�}�b�v�`�b�v�Ƃ̐ڐG����(�G�p)
		 * @param box				�Ώۂ̓����蔻��{�b�N�X
		 * @param pos				�Ώۂ̍��W
		 * @param move			�Ώۂ̈ړ���
		 * @param direction	�Ώۂ̌����t���O
		 * @return					�����o���������s�����ꍇ�͐ڐG���Ă���}�b�v�`�b�v�̌��ʔԍ���Ԃ�
		 *									�ڐG���Ă��Ȃ��ꍇ��-1��Ԃ�
		 */
		int IsHit(AABB box, Vector2& pos, Vector2& move, bool direction);
		/**
		 * @brief						�}�b�v�`�b�v�Ƃ̐ڐG����(�G�p)
		 * @param box				�Ώۂ̓����蔻��{�b�N�X
		 * @param pos				�Ώۂ̍��W
		 * @param move			�Ώۂ̈ړ���
		 * @param direction	�Ώۂ̌����t���O
		 * @param	isGran		�͂݃t���O
		 * @return					�����o���������s�����ꍇ�͐ڐG���Ă���}�b�v�`�b�v�̌��ʔԍ���Ԃ�
		 *									�ڐG���Ă��Ȃ��ꍇ��-1��Ԃ�
		 */
		int IsHit(AABB box, Vector2& pos, Vector2& move, bool direction, bool* isGran);
		/**
		 * @brief						�ӂƂ̐ڐG����
		 * @param box				�Ώۂ̓����蔻��{�b�N�X
		 * @param pos				�Ώۂ̍��W
		 * @param move			�Ώۂ̈ړ���
		 * @param ivx				�Ӄ`�b�v�̍��W���i�[����|�C���^
		 * @param direction	�Ώۂ̌����t���O
		 * @return 
		 */
		bool HitIvy(AABB box, Vector2 pos, Vector2 move, std::pair<double, double>* ivx, bool direction);
		/**
		 * @brief						���[���h���W���X�N���[�����W�ɃN�����v����
		 * @param	pos				���[���h���W
		 * @return					�N�����v�ɐ��������ꍇ��true��Ԃ�	
		 *									�N�����v�Ɏ��s�����ꍇ��false��Ԃ�
		 */
		bool Clamp(Vector2& pos);
		/**
		 * @brief						���X�N���[�����s�����̔���
		 * @return					���X�N���[�����s���ꍇ��true��Ԃ�
		 *									�s��Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsScrollX();
		/**
		 * @brief						�c�X�N���[�����s�����̔���
		 * @return					�c�X�N���[�����s���ꍇ��true��Ԃ�
		 *									�s��Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsScrollY();
		/**
		 * @brief						�}�b�v�`�b�v�̐؂�ւ�����
		 * @param						���̃X�e�[�W�L�[
		 */
		void ChangeMap(std::string nextStage);
		/**
		 * @brief						�}�b�v�T�C�Y(��)�̎擾
		 * @return					���݂̃}�b�v�̃}�b�v�T�C�Y(��)��Ԃ�
		 */
		inline int GetMapSizeWidth() { return _nowMap.MapSizeWidth() * _nowMap.ChipSizeWidth(); }
		/**
		 * @brief						�}�b�v�T�C�Y�̎擾
		 * @return					���݂̃}�b�v�̃}�b�v�T�C�Y��Ԃ�
		 */
		inline std::pair<int, int> GetMapSize() { return std::make_pair(_nowMap.MapSizeWidth(), _nowMap.MapSizeHeight()); }
		/**
		 * @brief						�`�b�v�J�E���g�̎擾
		 * @return					���݂̃}�b�v�̃`�b�v�J�E���g��Ԃ�
		 */
		inline std::tuple<int, int, int> GetChipCount() { return std::make_tuple(_nowMap.ChipCount(), _nowMap.ChipCountWidth(), _nowMap.ChipCountHeight()); }
		/**
		 * @brief						�}�b�v�`�b�v�T�C�Y�̎擾
		 * @return					���݂̃}�b�v�̃}�b�v�`�b�v�T�C�Y��Ԃ�
		 */
		inline std::pair<int, int> GetChipSize() { return std::make_pair(_nowMap.ChipSizeWidth(), _nowMap.ChipSizeHeight()); }
		/**
		 * @brief						���C���[�̎擾
		 * @return					���݂̃}�b�v�̃��C���[��Ԃ�
		 */
		inline int GetMapSizeLayer() { return _nowMap.MapSizeLayer(); }
		/**
		 * @brief						�t�@�C�����̎擾
		 * @return					���݂̃}�b�v�̃t�@�C������Ԃ�
		 */
		inline std::string GetFileChips() { return _nowMap.FileName(); }
		/**
		 * @brief						���[���h���W�̎擾
		 * @return					���[���h���W��Ԃ�
		 */
		inline Vector2 GetWorldPosition() { return _worldPosition; }
		/**
		 * @brief						���[���h���W�̈ړ��ʂ��擾
		 * @return					���݂̃t���[���ƑO�t���[���̃��[���h���W�̍�����Ԃ�
		 */
		inline Vector2 BeforeWorldPos() { return _worldPosition - _worldLast; }
		/**
		 * @brief						�X�e�[�W�J�ڃV�X�e���̎擾
		 * @return					�X�e�[�W�J�ڃV�X�e����Ԃ�
		 */
		inline std::unique_ptr<StageTransition>& GetStageTransition() { return _stageTransition; }
		/**
		 * @brief						���[���h���W�̍X�V
		 * @param	pos				���W
		 */
		void WorldUpdate(Vector2 pos);
	private:
		Game& _game;																				//!< �Q�[���N���X�̎Q��
		MapData _nowMap;																		//!< ���݂̃}�b�v�`�b�v���
		std::unique_ptr<MapDataManager> _mapManager;				//!< �}�b�v���̃}�l�[�W���[
		std::unique_ptr<ChipHitCheck> _chipCheck;						//!< �}�b�v�`�b�v�̓����蔻��Ǘ��p�}�l�[�W���[
		std::unique_ptr<StageTransition> _stageTransition;	//!< �X�e�[�W�J�ڗp�V�X�e��
		std::string _skey;																	//!< ���݂̃X�e�[�W
		std::string _chipKey;																//!< ���o���}�b�v�`�b�v�̃L�[
		Vector2 _worldPosition;															//!< ���[���h���W
		Vector2 _worldLast;																	//!< �O�t���[���̃��[���h���W
		/**
		 * @brief							���[���h���W���C�����邩�̔���
		 */
		void WorldClanp();
		/**
		 * @brief							Json�t�@�C���̓W�J
		 * @param fileName		Json�t�@�C����
		 * @return						�W�J�ɐ��������ꍇ�͕������Ԃ�
		 *										���s�����ꍇ��""��Ԃ�
		 */
		std::string StringFileLoad(std::string fileName);
		/**
		 * @brief							�}�b�v�f�[�^�̓ǂݍ���
		 * @param skey				�o�^�p�L�[
		 * @param filePath		�t�@�C���p�X
		 * @param strFileName	�t�@�C����
		 * @return						�ǂݍ��݂ɐ��������ꍇ��true��Ԃ�
		 *										���s�����ꍇ��0��Ԃ�
		 */
		int TiledJsonLoad(std::string skey, std::string filePath, std::string strFileName);
		/**
		 * @brief							�e�X�e�[�W�̈ꕔ�}�b�v�`�b�v�̔���͈͂��C��
		 */
		void SetChipsMap();
		/**
		 * @brief							�J�ڃ`�b�v�ƐڐG�������̔���
		 * @param no					�`�b�v�ԍ�
		 * @return						�ڐG�����ꍇ��true��Ԃ�
		 *										�ڐG���Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		bool TransitionResearch(const int no);
	};
}

