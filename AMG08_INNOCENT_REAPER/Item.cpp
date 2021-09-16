#include "Item.h"
#include "Game.h"
#include "MapChips.h"
#include "Particle_Image.h"
#include "ObjectServer.h"
#include "Player.h"

namespace inr {

	ImageValue::ImageValue(std::string key, Vector2 pos, int width, int height) {
		_key = key;
		_position = pos;
		_width = width;
		_height = height;
	}

	Item::Item(Game& game, std::string no) : _game(game), _pi(game) {
		_gkey = "";
		_position = { 0, 0 };
		_col = { _position, 0, 0, true };
		_stageNo = no;
	}

	void Item::Init() {

	}

	void Item::Process() {
		auto pbox = _game.GetObjectServer()->GetPlayer()->GetMainCollision();
		if (_col.HitCheck(pbox) == true) {
			_pi.DrawStart();
		} else { 
			_pi.DrawEnd();	// �`�揈���I��
		}
		_pi.Process();
	}

	void Item::Draw() {
		Vector2 xy = _position;
		_game.GetMapChips()->Clamp(xy);
		auto x = xy.IntX();
		auto y = xy.IntY();
		auto gh = graph::ResourceServer::GetHandles(_gkey, 0);
		DrawRotaGraph(x, y, 1.0, 0, gh, true, false);
		_pi.Draw();
	}

	void Item::SetParameter(ImageValue ivef, ImageValue ives) {
		_gkey = ivef.GraphKey();
		_position = ivef.Position();
		_col = { _position, ivef.Width(), ivef.Height() };
		_pi.SetParameter(ives.GraphKey(), ives.Position());
	}
}