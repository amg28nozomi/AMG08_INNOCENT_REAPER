#include "ObjectValue.h"

namespace inr {

	GimmickValue::GimmickValue(int type, int flag) {
		_gimmickType = type;
		_gimmickFlag = flag;
	}

	ObjectValue::ObjectValue() {
		_class = oscenario::OBJ_NULL;
		_spawnPos.clear();
		_soulType = 0;
		_gimmick = GimmickValue();
		_objType = IsObjectType(_class);
	}

	ObjectValue::ObjectValue(int classtype, Vector2 xy, int soulcolor, GimmickValue gvalue) {
		// äeéÌèÓïÒÇÃìoò^
		_class = classtype;
		_spawnPos.emplace_back(xy);
		_soulType = soulcolor;
		_gimmick = gvalue;
		_objType = IsObjectType(_class);
	}

	ObjectValue::ObjectValue(int classtype, std::vector<Vector2> xy, int soulcolor, GimmickValue gvalue) {
		_class = classtype;
		_spawnPos = xy;
		_soulType = soulcolor;
		_gimmick = gvalue;
		_objType = IsObjectType(_class);
	}

	int ObjectValue::IsObjectType(int classname) {
		switch (classname) {
		case oscenario::OBJ_PLAYER:
			return oscenario::type::PLAYER;
		case oscenario::OBJ_SOLDIER_DOLL:
		case oscenario::OBJ_BIG_DOLL:
		case oscenario::OBJ_CROW_DOLL:
			return oscenario::type::ENEMY;
		case oscenario::OBJ_SOUL:
			return oscenario::type::SOUL;
		case oscenario::OBJ_LEVER:
		case oscenario::OBJ_BLOCK:
		case oscenario::OBJ_CRYSTAL:
			return oscenario::type::GIMMICK;
		default:
			return -1;	// ÉoÉO
		}
	}

	void ObjectValue::PositionsUpdate(Vector2 newpos) {
		_spawnPos.clear();
		_spawnPos.emplace_back(newpos);
	}

	void ObjectValue::FlagUpdate(bool flag, int soul) {
		if (_gimmick.GimmickFlag() != flag) _gimmick.AddFlag(flag);
		if (soul == 0) return;
		if (_soulType != soul) _soulType = soul;
	}
}
