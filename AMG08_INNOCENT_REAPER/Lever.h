/*****************************************************************//**
 * \file   Lever.h
 * \brief  レバークラス（ギミックベースクラスのサブクラス）
 * 
 * \author 鈴木希海
 * \date   October 2021
 *********************************************************************/
#pragma once
#include "GimmickBase.h"

namespace inr {

	namespace gimmick {
		namespace door {
			constexpr auto NOT_DOOR = -1;
			constexpr auto D_LEVER = 1;
			constexpr auto D_RED = 2;
			constexpr auto D_BLUE = 3;
			constexpr auto D_BOSS = 4;
		}
	}

	class Door;

	class Lever : public GimmickBase {
	public:
		Lever(Game& game);
		~Lever() = default;

		void Init() override;
		void Process() override;
		void Draw() override;

		void OpenDoor();
		void SetParameter(ObjectValue objValue) override;	// 引数1:座標(レバー)　引数2;座標(ドア)
		void ObjValueUpdate() override;
	private:
		std::shared_ptr<Door> _door;
	};

}