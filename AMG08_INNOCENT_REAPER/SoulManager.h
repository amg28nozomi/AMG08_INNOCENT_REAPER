#pragma once
#include "SoulSkin.h"
#include <vector>
#include <unordered_map>

namespace inr {

	class Game;


	//// 魂の吐き出す情報
	//class SoulScenario {
	//	// color:色　owner:所有者は誰か 
	//	SoulScenario(std::string color);
	//};

	//class SoulManager {
	//public:
	//	using SoulOwner = std::unordered_map<std::string, Soul&>;

	//	static void Init();	// 初期化
	//	static void ClearSouls();	// 連想配列の要素消去
	//	static void LoadSoulOwner(SoulOwner& soulowner);	// 登録処理
	//	static void OwnerChange(std::string lastowner, std::string nextowner);	// 所有者の切り替え

	//private:
	//	static SoulOwner _souls;	// 魂一覧
	//};
}

