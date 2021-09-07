#pragma once
#include <vector>
#include "Vector2.h"

namespace inr {

	namespace oscenario {
		constexpr auto OBJ_NULL = -1;
		constexpr auto OBJ_PLAYER = 0;
		constexpr auto OBJ_SOLDIER_DOLL = 1;
		constexpr auto OBJ_BIG_DOLL = 2;
		constexpr auto OBJ_CROW_DOLL = 3;
		constexpr auto OBJ_SOUL = 4;
		constexpr auto OBJ_LEVER = 5;
		constexpr auto OBJ_BLOCK = 6;
		constexpr auto OBJ_CRYSTAL = 7; // quartz

		namespace type {
			constexpr auto PLAYER = 0;
			constexpr auto ENEMY = 1;
			constexpr auto SOUL = 2;
			constexpr auto GIMMICK = 3;
		}

		namespace gimmick {
			constexpr auto TYPE_NULL = -1;
			constexpr auto TYPE_LEVER = 0;
			constexpr auto TYPE_CRYSTAL = 1;
			constexpr auto TYPE_BLOCK = 2;
			constexpr auto TYPE_DOOR = 3;

			constexpr auto FLAG_NULL = -1;
			constexpr auto FLAG_FALSE = 0;
			constexpr auto FLAG_TRUE = 1;
		}
	}

	// ギミックの情報
	class GimmickValue {
	public:
		GimmickValue(int type = oscenario::gimmick::TYPE_NULL, int flag = oscenario::gimmick::FLAG_NULL);
		inline int GimmickType() { return _gimmickType; }
		inline int GimmickFlag() { return _gimmickFlag; }
	private:
		int _gimmickType;	// ギミックの種類（-1:対象外　0:レバー　1:水晶　2:　3:ドア）
		int _gimmickFlag;	// ギミックのフラグ（-1:対象外　0:false　1:true）
	};

	// 登録情報
	class ObjectValue {
	public:
		ObjectValue();
		ObjectValue(int classtype, Vector2 xy, int soulcolor = 0, GimmickValue gvalue = GimmickValue());
		ObjectValue(int classtype, std::vector<Vector2> xy, int soulcolor = 0, GimmickValue gvalue = GimmickValue());
		~ObjectValue() = default;

		inline int ClassName() { return _class; }
		inline int SoulType() { return _soulType; }
		inline int GimmickType() { return _gimmick.GimmickType(); }
		inline int GimmickFlag() { return _gimmick.GimmickFlag(); }
		inline int ObjectType() { return _objType; }
		inline std::vector<Vector2> Positions() { return _spawnPos; }
		
	private:
		int _class;	// 生成するクラスは何か
		std::vector<Vector2> _spawnPos;	// 出現地点
		int _soulType;	// 魂の有無（0:未所持　1:赤　2:青）
		GimmickValue _gimmick;	// ギミックの登録情報
		int _objType;	// オブジェクトの種類

		int IsObjectType(int classname);	// このオブジェクトは何か？
	};
}

