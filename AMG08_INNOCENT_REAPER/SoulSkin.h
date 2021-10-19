/*****************************************************************//**
 * @file   SoulSkin.h
 * @brief	 ���N���X
 * 
 * @author ��؊�C
 * @date   October 2021
 *********************************************************************/
#pragma once
#include "ObjectBase.h"
#include "Vector2.h"

namespace inr {
	/** ���̃R���p�C�����萔 */
	namespace soul {
		constexpr auto RED_SOUL = "r_soul";		//!< ��
		constexpr auto BLUE_SOUL = "b_soul";	//!< ��
		constexpr auto R_FLOAT = "r_float";		//!< ���V(��)
		constexpr auto B_FLOAT = "b_float";		//!< ���V(��)
		// �^�C�v
		constexpr auto RED = false;						//!< ��
		constexpr auto BLUE = true;						//!< ��
		constexpr auto IMAGE_SIZE = 100;			//!< ���̃T�C�Y
		// �e���[�V�����̑�������
		constexpr auto SF_BLUE_SOUL = 1;			//!< �����̃��[�V������
		constexpr auto SF_B_SOUL = 4;					//!< ���V��������̃��[�V������
		constexpr auto SF_RED_SOUL = 1;				//!< �Ԃ����̃��[�V������
		constexpr auto SF_R_SOUL = 5;					//!< ���V����Ԃ����̃��[�V������
	}
	/** ���N���X */
	class SoulSkin : public ObjectBase {
	public:
		/**
		 * @brief							�R���X�g���N�^
		 * @param game				�Q�[���N���X�̎Q��
		 */
		SoulSkin(Game& game);
		/**
		 * @brief							�f�X�g���N�^
		 */
		~SoulSkin() override;
		/**
		 * @brief							����������
		 */
		void Init() override;
		/**
		 * @brief							�X�V����
		 */
		void Process() override;
		/**
		 * @brief							�`�揈��
		 */
		void Draw() override;
		/**
		 * @brief							���̃Z�b�g�A�b�v
		 * @param scolor			���̐F
		 */
		inline void SoulChange(bool scolor) { (scolor == soul::RED) ? _sType = Type::RED : _sType = Type::BLUE; }
	private:
		/** ���̐F��\���񋓌^ */
		enum class Type {
			RED, BLUE
		};
		Type _sType;					//!< �^�C�v
		Vector2 _moveVector;	//!< �ړ���
		double _speed;				//!< �ړ����x
		double _space;				//!< �Ԋu
		bool _active;					//!< ���̉����Ă��邩�H
		bool _give;						//!< ���L�������n���邩
		bool _isOwner;				//!< ���L�҂͂��邩�H
		/**
		 * @brief							�ǐՏ���
		 */
		void Tracking();
		/**
		 * @brief							�ړ�����
		 */
		void Move();
		/**
		 * @brief							���L���̕t�^(���L�҂����Ȃ�������j
		 */
		void Give();
	public:
		/**
		 * @brief							�^�C�v�̎擾
		 * @return						�^�C�v��Ԃ�
		 */
		inline Type GetType() { return _sType; }
		/**
		 * @brief							���̐ݒ�
		 * @param spawn				�������W
		 * @param soulcolor		�F
		 */
		void SetStatus(Vector2 spawn, std::string soulcolor)override;
		/**
		 * @brief							�ړ����x�̓o�^
		 * @param speed				�ړ����x
		 */
		void SetSpeed(double speed) override { _speed = speed; }
		/**
		 * @brief							���̐ݒ�
		 * @param soulcolor		�F
		 * @param speed				�ړ����x
		 */
		void SetParameter(int soulcolor, double speed);	
		/**
		 * @brief							���̉�����
		 * @param spawn				�����n�_
		 */
		void SetSpwan(Vector2 spawn);
		/**
		 * @brief							�񊈓���ԂɈڍs
		 */
		inline void Inactive() { _active = false; }
		/**
		 * @brief							������ԂɈڍs
		 */
		inline void Active() { _active = true; }
		/**
		 * @brief							�Ǐ]�Ԋu�̓o�^
		 */
		inline void Space(double space) { _space = space; }
		/**
		 * @brief							���L�҃t���O��؂�
		 */
		inline void OwnerNull() { _isOwner = false; }
		/**
		 * @brief							���L�����n�t���O�̎擾
		 * @return						���L�����n�t���O��Ԃ�
		 */
		inline bool IsGive() { return _give; }
		/**
		 * @brief							���L�҃t���O�̎擾
		 * @return						���L�҃t���O��Ԃ�
		 */
		inline bool IsOwner() { return _isOwner; }
		/**
		 * @brief							���̐F�̎擾
		 * @return						���̐F��Ԃ�
		 */
		bool SoulColor();
	};
}

