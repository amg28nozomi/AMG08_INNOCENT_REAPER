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

		inline bool IsSwitch() { return _switch == true; }	// スイッチは入力可能か否か
	private:
		enum class DoorType {
			// レバー、水晶(赤)、水晶(青)
			LEVER, CRYSTAL_RED, CRYSTAL_BLUE
		};
		int _pal;	// 透明度
		bool _switch;
	};
}

