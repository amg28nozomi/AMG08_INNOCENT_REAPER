#pragma once
#include "GimmickBase.h"

namespace inr {

	// �h�A(�M�~�b�N)
	class Door : public GimmickBase {
	public:
		Door(Game& game);
		~Door() = default;
		// ������
		void Init() override;
		// �X�V
		void Process() override;
		// �`��
		void Draw() override;
		// �I�u�W�F�N�g���̐ݒ�
		void SetParameter(Vector2 spwan, std::string key, int flag);
		// �I�u�W�F�N�g���̓o�^
		void SetParameter(ObjectValue objValue) override;
		// �J�t���O�̋N��
		void SwitchOn();
		// �J�t���O�}��
		void SwitchOff();
		// �I�u�W�F�N�g�̉����o������(����1:�Ώۂ̓����蔻��@����2:�Ώۂ̍��W�@����3:�ړ��x�N�g���@����4:�����@����5:���O�Ɍ��������]������)
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection);
		// �J�t���O�̎擾
		inline bool IsSwitch() { return _switch; }
		// ���̐F�̎擾
		inline int DoorColor() { return _color; }
	private:
		int _color;			// �h�A�̐F
		double _normalY;	// �ʏ���W
		bool _switch;		// �J�t���O
		bool _ismove;		// �ړ������H
		Vector2 _moves;
		// ���F�̔��Ȃ̂��̔���
		void SetColor(std::string key);
		// �A�j���[�V��������
		bool MotionCount();
		// �h�A�̈ړ�����
		bool DoorMove();
	};
}

