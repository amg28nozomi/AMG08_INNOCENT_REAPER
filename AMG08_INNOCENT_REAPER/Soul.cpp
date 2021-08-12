#include "Soul.h"
#include "ObjectBase.h"
#include "Vector2.h"
#include "game.h"
#include "ObjectServer.h"

namespace inr {

	Soul::Soul(Game& game) : ObjectBase(game) {
		_sType = Type::RED;
	}

	Soul::~Soul() {

	}

	void Soul::Process() {

	}

	void Soul::Draw() {

	}

	void Soul::Tracking() {
		// プレイヤーの参照を取得
		auto player = _game.GetObjectServer()->GetPlayer();
		auto sx = _position.GetX();
		auto sy = _position.GetY();
		auto px = player.GetPosition().GetX();
		auto py = player.GetPosition().GetY();
		// プレイヤーの参照を取得
		auto player = _game.GetObjectServer()->GetPlayer();
		// 自身とプレイヤー間のベクトルを算出
		Vector2 mv = { px - sx, py - sy };
	}
}
