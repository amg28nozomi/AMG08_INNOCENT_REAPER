#pragma once
#include "GimmickBase.h"

namespace inr {
	namespace gimmick {
		namespace block {
			// ��ꂽ���ǂ����̃t���O
			constexpr auto BRAKE_ON = true;	// �j��ς�
			constexpr auto BRAKE_OFF = false;	// ���j��
		}
	}

	// �����(�M�~�b�N)
	class Block : public GimmickBase {
	public:
		Block(Game& game);
		~Block() = default;
			
		// ������
		void Init() override;
		// �X�V
		void Process() override;
		// �`��
		void Draw() override;

		// �I�u�W�F�N�g���̓o�^
		void SetParameter(ObjectValue objValue) override;
		// �I�u�W�F�N�g���̍X�V
		void ObjValueUpdate() override;

		// �I�u�W�F�N�g�͉��Ă��邩�H
		inline bool IsBreak() { return _break; }
		// �����o������
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection);
		// �j�󏈗�
		bool Break();
	private:
		int _pal;	// �����x
		bool _break;	// �󂳂ꂽ���ۂ�
	};
}

