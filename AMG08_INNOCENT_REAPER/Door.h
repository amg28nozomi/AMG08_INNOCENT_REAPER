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

		void SetParameter(Vector2 spwan, std::string key);
		void SwitchOn();

		inline bool IsSwitch() { return _switch == true; }	// �X�C�b�`�͓��͉\���ۂ�
	private:
		enum class DoorType {
			// ���o�[�A����(��)�A����(��)
			LEVER, CRYSTAL_RED, CRYSTAL_BLUE
		};
		int _pal;	// �����x
		bool _switch;
	};
}

