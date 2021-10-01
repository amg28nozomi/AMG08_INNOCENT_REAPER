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
		void SwitchOff();	// 扉を閉める

		void SetParameter(ObjectValue objValue) override;

		bool Extrude(AABB box, Vector2& pos, Vector2& move, bool direction, bool changedirection);

		inline bool IsSwitch() { return _switch; }	// スイッチは入力可能か否か
		inline int DoorColor() { return _color; }
	private:
		enum class DoorType {
			// レバー、水晶(赤)、水晶(青)
			LEVER, CRYSTAL_RED, CRYSTAL_BLUE
		};
		int _color;	// ドアの色
		double _normalY;
		bool _switch;
		bool _ismove;	// 移動中か？
		Vector2 _moves;

		void SetColor(std::string key);

		bool DoorMove();	// ドアの移動処理
	};
}

