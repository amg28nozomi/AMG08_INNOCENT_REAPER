#pragma once
#include "GimmickBase.h"

namespace inr {
	// ��ꂽ���ǂ����̃t���O
	namespace gimmick {
		namespace block {
			constexpr auto BRAKE_ON = true;	// �j��ς�
			constexpr auto BRAKE_OFF = false;	// ���j��
		}
	}
	// �����
	class Block : public GimmickBase {
	public:
		Block(Game& game);
		~Block() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(ObjectValue objValue) override;	// �I�u�W�F�N�g���̓o�^
		void ObjValueUpdate() override;	// �I�u�W�F�N�g���̍X�V

		inline bool IsBreak() { return _break; }	// �I�u�W�F�N�g�͉��Ă��邩�H
		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection);	// �����o������
		bool Break();	// �j�󏈗�
	private:
		int _pal;	// �����x
		bool _break;	// �󂳂ꂽ���ۂ�
	};
}

