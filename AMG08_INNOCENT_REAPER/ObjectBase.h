/*****************************************************************//**
 * @file   ObjectBase.h
 * @brief  �I�u�W�F�N�g�̃X�[�p�[�N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <unordered_map>
#include <DxLib.h>
#include "Collision.h"
#include "AABB.h"
#include "ResourceServer.h"
#include "ObjectValue.h"

/** ��d�C���N���[�h�h�~ */
class Vector2;

namespace {
	/** �L�[:��ԃL�[�@�l(����):�����[�V�����t���[�����@�l(�E��):SE�̍Đ��t���[�� */
	using KeyDatas = std::unordered_map<std::string, std::pair<int, int>>;
	/** ����:�摜�L�[�@�E��:�T�E���h�L�[ */
	using Keys = std::pair<std::string, std::string>;
}
namespace inr {
	constexpr auto FRAME_G = 0.5;		//!< 1�t���[�����̏d�͉����l
	constexpr auto MAX_G = 15.0;		//!< �d�͒l�̏��
	constexpr auto SE_NUM = 0;			//!< SE�t���[������
	/** ��d�C���N���[�h�h�~ */
	class Game;
	class ObjectServer;
	/** �I�u�W�F�N�g�x�[�X */
	class ObjectBase {
		/** �t�����h�w�� */
		friend class ObjectServer;
	protected:
		/**
		 * @brief				�I�u�W�F�N�g�̎�ނ�\���񋓌^
		 *							�I�u�W�F�N�g�x�[�X�@���@�@�G�@���@�M�~�b�N
		 */
		enum class ObjectType {
			OBJECT_BASE, PLAYER, ENEMY, SOUL, GIMMICK
		};
		Game& _game;					//!< �Q�[���Q��
		ObjectType _type;			//!< �I�u�W�F�N�g�^�C�v
		int _aCount;					//!< �A�j���[�V�����̓Y����
		double _speed;				//!< �ړ����x
		double _gravity;			//!< �d�͂ɂ������x
		bool _direction;			//!< �����t���O
		bool _changeGraph;		//!< �摜�̐؂�ւ��t���O
		bool _stand;					//!< �n�ʂɗ����Ă��邩�ǂ���
		bool _delete;					//!< �f���[�g���邩�ۂ��H�itrue�FObjectServer��������@false:�������j
		bool _isJump;					//!< �W�����v�t���O
		ObjectValue _oValue;	//!< �������̏��
		Vector2 _position;		//!< ���W
		Vector2 _lastChip;		//!< �Ō�ɗ����Ă����}�b�v�`�b�v
		AABB _mainCollision;	//!< �L�����̓����蔻��
		Keys _divKey;					//!< ����:�O���t�B�b�N�p�A�E��:�T�E���h�p
		KeyDatas _motionKey;	//!< first:�L�[, s.first:���t���[����, SE�̍Đ�����
		/**
		 * @brief							ResourceServer����O���t�B�b�N�n���h���擾
		 * @param gh					int�^�̃A�h���X
		 * @return						�摜�؂�ւ�����������ꍇ��true��Ԃ�
		 *										�ʏ�̓ǂݍ��݂̏ꍇ��false��Ԃ�
		 */
		virtual bool GraphResearch(int* gh);
		/**
		 * @brief							�}�b�v�`�b�v�̏�ɗ����Ă��邩�H
		 * @return						�����Ă���ꍇ��true��Ԃ�
		 *										�����Ă��Ȃ��ꍇ��false��Ԃ�
		 */
		virtual bool IsStandChip();
		/**
		 * @brief							�����[�V�����t���[�����̎擾
		 * @param key					��ԃL�[
		 * @return						�q�b�g�����ꍇ�͑��t���[������Ԃ�
		 *										�q�b�g���Ȃ������ꍇ��-1��Ԃ�
		 */
		virtual int GetSize(const std::string& key);
		/**
		 * @brief							�A�j���[�V�����J�E���^�̑���
		 */
		virtual void AnimationCount();
		/**
		 * @brief							�A�j���[�V�����̕`��Ԋu
		 * @return						�Ԋu��Ԃ�
		 */
		int AnimationInterval();
		/**
		 * @brief							���݂̃A�j���[�V�����ԍ��̎擾
		 * @return						�A�j���[�V�����ԍ���Ԃ�
		 */
		int AnimationNumber();
		/**
		 * @brief							�A�j���[�V�����ԍ��͍ő傩
		 * @return						�ő�̏ꍇ��true��Ԃ�
		 *										�ő�ł͂Ȃ��ꍇ��false��Ԃ�
		 */
		bool IsAnimationMax();
		/**
		 * @brief							�A�j���[�V�����J�E���g�͍ő傩
		 * @return						�ő�̏ꍇ��true��Ԃ�
		 *										�ő�ł͂Ȃ��ꍇ��false��Ԃ�
		 */
		bool AnimationCountMax();
		/**
		 * @brief							SoundServer����T�E���h�n���h�����擾����
		 * @param key					�L�[
		 * @return						�擾�����T�E���h�n���h����Ԃ�
		 *										�擾�Ɏ��s�����ꍇ��-1��Ԃ�
		 */
		virtual int SoundResearch(const std::string& key);
		/**
		 * @brief							�Đ��t���[�����̎擾
		 * @param key					�L�[
		 * @return						�q�b�g�����ꍇ�͍Đ��t���[������Ԃ�
		 *										�q�b�g���Ȃ������ꍇ��-1��Ԃ�
		 */
		virtual int GetSoundFrame(const std::string& key);
		/**
		 * @brief							���݂̓����蔻��̎擾
		 * @param	key					���݂̏�Ԃ�\���L�[
		 * @return						��̓����蔻��{�b�N�X��Ԃ�
		 */
		virtual AABB NowCollision(std::string key);
		/**
		 * @brief							���̃I�u�W�F�N�g�͎��@���ǂ���
		 * @return						���@�̏ꍇ��true��Ԃ�
		 *										���@�ł͂Ȃ��ꍇ��false��Ԃ�
		 */
		bool ThisPlayer();
		/**
		 * @brief							�M�~�b�N�Ƃ̏Փ˔���
		 * @param move				�ړ���
		 */
		void GimmickCheck(Vector2& move);
#ifdef _DEBUG
		/**
		 * @brief							�����蔻��{�b�N�X�̕`��(�f�o�b�O���[�h����)
		 * @param cb					�`�悷�锻��{�b�N�X
		 * @param color				�F
		 */
		void DrawDebugBox(Collision cb, int color = GetColor(255, 0, 255));
#endif
	public:
		/**
		 * @brief							�R���X�g���N�^
		 * @param game				�Q�[���N���X�̎Q��
		 */
		ObjectBase(Game& game);
		/**
		 * @brief							�f�X�g���N�^
		 */
		virtual ~ObjectBase();
		/**
		 * @brief							����������
		 */
		virtual void Init();
		/**
		 * @brief							�X�V����
		 */
		virtual void Process();
		/**
		 * @brief							�`�揈��
		 */
		virtual void Draw();
		/**
		 * @brief							�����̎擾
		 * @return						�����t���O��Ԃ�
		 */
		inline bool GetDirection() { return _direction; }
		/**
		 * @brief							�����t���O�̎擾
		 * @return						�����t���O��Ԃ�
		 */
		inline bool IsDelete() { return _delete; }
		/**
		 * @brief							�I�u�W�F�N�g�^�C�v�̎擾
		 * @return						�I�u�W�F�N�g�^�C�v��Ԃ�
		 */
		inline ObjectType GetType() { return _type; }
		/**
		 * @brief							���W�̎擾
		 * @return						���W��Ԃ�
		 */
		inline Vector2 GetPosition() { return _position; }
		/**
		 * @brief							�ړ��ʂ̎擾
		 * @return						��̈ړ��x�N�g����Ԃ�
		 */
		inline virtual Vector2 GetMoveVector() { return Vector2(); }
		/**
		 * @brief							�ʏ퓖���蔻��{�b�N�X�̎擾
		 * @return						�ʏ퓖���蔻��{�b�N�X��Ԃ�
		 */
		inline AABB& GetMainCollision() { return _mainCollision; }
		/**
		 * @brief							�I�u�W�F�N�g���̎擾
		 * @return						�I�u�W�F�N�g����Ԃ�
		 */
		inline ObjectValue GetObjectValue() { return _oValue; }
		/**
		 * @brief							���S����
		 * @return						false��Ԃ�
		 */
		virtual bool IsDead();
		/**
		 * @brief							�I�u�W�F�N�g���̓o�^
		 * @param ObjectValue �I�u�W�F�N�g���
		 */
		virtual void SetParameter(ObjectValue objValue);
		/**
		 * @brief							��Ԃ̐ݒ�
		 * @param spawn				�������W
		 * @param soulcolor		���̐F
		 */
		virtual void SetStatus(Vector2 spawn, std::string soulcolor) { return; }
		/**
		 * @brief							�ړ����x�̐ݒ�
		 * @param speed				�ړ����x
		 */
		virtual void SetSpeed(double speed) { return; }
		/**
		 * @brief							�����t���O�̕ύX
		 */
		void Del();
		/** �����蔻����i�[����A�z�z��(�L�[:��ԁ@�l:�����蔻��{�b�N�X) */
		using boxs = std::unordered_map<std::string, AABB>;
	};
}
