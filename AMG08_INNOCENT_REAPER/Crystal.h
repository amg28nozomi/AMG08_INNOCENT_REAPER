#pragma once
#include "GimmickBase.h"
#include "SoulSkin.h"
#include <memory>
#include <vector>

namespace inr {

	class Door;

	// ����(�M�~�b�N)
	class Crystal : public GimmickBase {
	public:
		Crystal(Game& game);
		~Crystal() = default;

		// �`��
		void Draw() override;

		// �I�u�W�F�N�g���̓o�^
		void SetParameter(ObjectValue objValue) override;
		// �I�u�W�F�N�g���̍X�V
		void ObjValueUpdate() override;
		// ���@�A�N�V�����Ƃ̏Փ˔���
		void CollisionHit(const std::string ckey, Collision acollision, bool direction);

	private:
		std::shared_ptr<SoulSkin> _soul;	// �ێ����Ă��鍰
		std::vector<std::shared_ptr<Door>> _doors;	// �ێ����Ă���h�A

		// �h�A�̐ݒ�
		void SetDoors(std::vector<std::string> key);
		// �`��摜�̐ݒ�
		void GraphKey();
		// ���̐���
		void AddSoul();
		// �h�A�̉��
		bool IsOpen(int crystal);

		
	};
}

