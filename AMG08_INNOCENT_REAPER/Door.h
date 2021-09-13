#pragma once
#include "GimmickBase.h"

namespace inr {

	class Door : public GimmickBase {
	public:
		Door(Game& game);
		~Door() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void SetParameter(Vector2 spwan, std::string key, int flag);
		void SwitchOn();
		void SwitchOff();	// ����߂�

		void SetParameter(ObjectValue objValue) override;

		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection);

		inline bool IsSwitch() { return _switch; }	// �X�C�b�`�͓��͉\���ۂ�
		inline int DoorColor() { return _color; }
	private:
		enum class DoorType {
			// ���o�[�A����(��)�A����(��)
			LEVER, CRYSTAL_RED, CRYSTAL_BLUE
		};
		int _pal;	// �����x
		int _color;	// �h�A�̐F
		bool _switch;

		void SetColor(std::string key);
	};
}

