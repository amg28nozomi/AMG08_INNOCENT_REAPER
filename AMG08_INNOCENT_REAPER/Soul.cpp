#include "Soul.h"
#include "ObjectBase.h"
#include "Vector2.h"
#include "game.h"
#include "ObjectServer.h"

namespace inr {

	Soul::Soul(Game& game) : ObjectBase(game), _moveVector() {
		_sType = Type::RED;
	}

	Soul::~Soul() {

	}

	void Soul::Process() {
		Tracking();
		Move();
	}

	void Soul::Draw() {

	}

	void Soul::Tracking() {
		// プレイヤーの参照を取得
		auto player = _game.GetObjectServer()->GetPlayer();
		auto px = player.GetPosition().GetX();
		auto py = player.GetPosition().GetY();
		// 自身とプレイヤー間のベクトルを算出
		Vector2 mv = { px - _position.GetX(), py - _position.GetY() };
		mv.Normalize();	// ベクトルの正規化
		// 移動ベクトルに加算（）
		_moveVector.GetPX() = mv.GetX();
		_moveVector.GetPY() = mv.GetY();
	}

	void Soul::Move() {
		_position + _moveVector;	// 座標更新
	}
}
