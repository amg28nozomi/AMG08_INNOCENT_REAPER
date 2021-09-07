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
	}

	ObjectValue::ObjectValue(int classtype, Vector2 xy, int soulcolor, GimmickValue gvalue) {
		// äeéÌèÓïÒÇÃìoò^
		_class = classtype;
		_spawnPos.emplace_back(xy);
		_soulType = soulcolor;
		_gimmick = gvalue;
	}

	ObjectValue::ObjectValue(int classtype, std::vector<Vector2> xy, int soulcolor, GimmickValue gvalue) {
		_class = classtype;
		_spawnPos = xy;
		_soulType = soulcolor;
		_gimmick = gvalue;
	}
}
