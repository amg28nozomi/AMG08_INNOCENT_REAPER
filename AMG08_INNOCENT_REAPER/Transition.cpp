/*****************************************************************//**
 * @file   Transition.cpp
 * @brief  ステージ遷移情報
 *
 * @author 鈴木希海
 * @date   October 2021
 *********************************************************************/
#include "Transition.h"

namespace inr {
	// コンストラクタ
	Transition::Transition(std::string stageKey, Vector2 position, bool direction) {
		_nextKey = stageKey;
		_position = position;
		_direction = direction;
	}
}
